package dev.dmotservices.reflection;


import javax.inject.Inject;
import java.lang.annotation.Annotation;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.*;

public class Reflection {

    /**
     * Iterates over each field and method of a class in search of a particular annotation class.
     * The annotation handler is triggered when the annotation is present.
     *
     * @param targetObject      The object to be processed.
     * @param annotationClass   The annotation class of interest.
     * @param annotationHandler The handler which defines the actions to be carried out on the fields and methods.
     */
    public static void processAnnotation(Object targetObject, Class<?> annotationClass, AnnotationHandler annotationHandler) {
        if (null == annotationClass)
            throw new IllegalArgumentException("An annotation class must be specified.");
        if (null == annotationHandler)
            throw new IllegalArgumentException("An annotation handler must be specified.");
        Map<Class<?>, AnnotationHandler> annotationClassHandlerMap = new HashMap<>();
        annotationClassHandlerMap.put(annotationClass, annotationHandler);
        processAnnotationCollection(targetObject, annotationClassHandlerMap);
    }

    /**
     * Iterates over each field and method of a class in search of the annotations held in the collection.
     * The associated annotation handler is triggered when each annotation is present.
     *
     * @param targetObject              The object to be processed.
     * @param annotationClassHandlerMap The collection of annotations of interested and their handlers.
     */
    public static void processAnnotationCollection(Object targetObject, Map<Class<?>, AnnotationHandler> annotationClassHandlerMap) {
        if (null == targetObject)
            throw new IllegalArgumentException("A target object is required.");
        if (null == annotationClassHandlerMap)
            throw new IllegalArgumentException("A collection of annotation classes and handlers is required.");
        for (Field field : getCombinedFields(targetObject.getClass())) {
            for (Annotation annotation : field.getDeclaredAnnotations()) {
                Class<?> clazz = annotation.annotationType();
                if (annotationClassHandlerMap.containsKey(clazz)) {
                    annotationClassHandlerMap.get(clazz).process(field);
                }
            }
        }
        for (Method method : getCombinedMethods(targetObject.getClass())) {
            for (Annotation annotation : method.getDeclaredAnnotations()) {
                Class<?> clazz = annotation.annotationType();
                if (annotationClassHandlerMap.containsKey(clazz)) {
                    annotationClassHandlerMap.get(clazz).process(method);
                }
            }
        }
    }

    /**
     * Injects a value into an object's field only if it has been annotated with @Inject.
     *
     * @param targetObject the object whose private or protected member variables are being modified.
     * @param fieldName    the name of variable whose value is to be changed.
     * @param fieldValue   the value to assign to the variable.
     * @return true if successful, false otherwise.
     */
    public static boolean injectValue(Object targetObject, String fieldName, Object fieldValue) {
        return injectValueByCriteria(field -> {
            final Annotation annotation = field.getAnnotation(Inject.class);
            if (null == annotation)
                return false;
            String discoveredName = field.getName();
            return discoveredName.equals(fieldName);
        }, fieldName, targetObject, fieldValue);

    }

    /**
     * Injects a value into an object's field irrespective of whether it has been annotated or not.
     *
     * @param targetObject the object whose private or protected member variables are being modified.
     * @param fieldName    the name of variable whose value is to be changed.
     * @param fieldValue   the value to assign to the variable.
     * @return true if successful, false otherwise.
     */
    @SuppressWarnings("UnusedReturnValue")
    public static boolean injectValueBruteForce(Object targetObject, String fieldName, Object fieldValue) {
        return injectValueByCriteria(field -> {
            String discoveredName = field.getName();
            return discoveredName.equals(fieldName);
        }, fieldName, targetObject, fieldValue);
    }

    /**
     * Injects a set of key-value pairs (field names, value) only if they have been annotated with @Inject.
     *
     * @param targetObject the object whose private or protected member variables are being modified.
     * @param keyValueMap  a map of the field names and the values to be assigned.
     * @return true if successful, false otherwise.
     */
    public static boolean massInject(Object targetObject, Map<String, Object> keyValueMap) {
        return injectValueByCriteria(field -> {
            final Annotation annotation = field.getAnnotation(Inject.class);
            return (null != annotation);    // @Inject is present if annotation has a value.
        }, targetObject, keyValueMap);
    }

    /**
     * Injects a set of key-value pairs (field names, value) irrespective of whether they have been annotated or not.
     *
     * @param targetObject the object whose private or protected member variables are being modified.
     * @param keyValueMap  a map of the field names and the values to be assigned.
     * @return true if successful, false otherwise.
     */
    @SuppressWarnings("UnusedReturnValue")
    public static boolean massInjectBruteForce(Object targetObject, Map<String, Object> keyValueMap) {
        return injectValueByCriteria(field -> true, targetObject, keyValueMap);
    }

    /**
     * Injects a value into an object's field irrespective of whether it has been annotated or not
     * according to a specific set of critieria.
     *
     * @param fieldChecker provides the criteria which should be used to check each field.
     * @param fieldName    the name of variable whose value is to be changed.
     * @param fieldValue   the value to assign to the variable.
     * @param targetObject the object whose private or protected member variables are being modified.
     * @return true if successful, false otherwise.
     */
    public static boolean injectValueByCriteria(FieldChecker fieldChecker, String fieldName, Object targetObject, Object fieldValue) {
        if (null == fieldName)
            return false;
        Map<String, Object> keyValueMap = new HashMap<>();
        keyValueMap.put(fieldName, fieldValue);
        return injectValueByCriteria(fieldChecker, targetObject, keyValueMap);
    }

    /**
     * Injects a value into an object's field irrespective of whether it has been annotated or not
     * according to a specific set of critieria.
     *
     * @param fieldChecker provides the criteria which should be used to check each field.
     * @param keyValueMap  a map of the field names and the values to be assigned.
     * @param targetObject the object whose private or protected member variables are being modified.
     * @return true if successful, false otherwise.
     */
    public static boolean injectValueByCriteria(FieldChecker fieldChecker, Object targetObject, Map<String, Object> keyValueMap) {
        if (null == fieldChecker)
            return false;
        if (null == keyValueMap)
            return false;
        if (keyValueMap.isEmpty())
            return false;
        if (null == targetObject)
            return false;
        try {
            for (Field field : getCombinedFields(targetObject.getClass())) {
                if (!fieldChecker.isRequiredField(field))
                    continue;
                if (keyValueMap.containsKey(field.getName())) {
                    final boolean accessible = field.canAccess(targetObject);
                    field.setAccessible(true);
                    field.set(targetObject, keyValueMap.get(field.getName()));
                    field.setAccessible(accessible);
                }
            }
        } catch (IllegalAccessException e) {
            return false;
        }
        return true;
    }

    /**
     * Returns the annotations and the parent annotations of this class.
     *
     * @param clazz the class to be inspected.
     * @return the annotations and parent annotations of this class.
     */
    public static Annotation[] getCombinedAnnotations(Class<?> clazz) {
        Annotation[] theseAnnotations = getAnnotations(clazz);
        if (null == clazz.getSuperclass())
            return theseAnnotations;
        Annotation[] parentsAnnotations = getAnnotations(clazz.getSuperclass());
        Annotation[] combinedAnnotations = new Annotation[0];
        if (null != theseAnnotations && null != parentsAnnotations) {
            combinedAnnotations = new Annotation[theseAnnotations.length + parentsAnnotations.length];
            System.arraycopy(theseAnnotations, 0, combinedAnnotations, 0, theseAnnotations.length);
            System.arraycopy(parentsAnnotations, 0, combinedAnnotations, theseAnnotations.length, parentsAnnotations.length);
        }
        return combinedAnnotations;
    }

    /**
     * Returns the annotations of this class.
     *
     * @param clazz the class to be inspected.
     * @return the annotations of this class.
     */
    public static Annotation[] getAnnotations(Class<?> clazz) {
        if (null == clazz)
            return new Annotation[0];
        List<Annotation[]> annotationArrayList = new ArrayList<>();
        annotationArrayList.add(clazz.getAnnotations());
        annotationArrayList.add(clazz.getDeclaredAnnotations());
        for (Field field : clazz.getDeclaredFields()) {
            annotationArrayList.add(field.getDeclaredAnnotations());
        }
        for (Method method : clazz.getDeclaredMethods()) {
            annotationArrayList.add(method.getDeclaredAnnotations());
        }
        for (Constructor<?> constructor : clazz.getDeclaredConstructors()) {
            annotationArrayList.add(constructor.getDeclaredAnnotations());
        }
        Annotation[] combined = new Annotation[0];
        for (Annotation[] annotations : annotationArrayList) {
            if (null == annotations || annotations.length < 1)
                continue;
            final int annotationsLength = annotations.length;
            final int combinedLength = combined.length;
            final int lengthAfterwards = annotationsLength + combinedLength;
            Annotation[] combinedReplacement = new Annotation[lengthAfterwards];
            try {
                System.arraycopy(combined, 0, combinedReplacement, 0, combined.length);
                System.arraycopy(annotations, 0, combinedReplacement, combined.length, annotations.length);
            } catch (ArrayIndexOutOfBoundsException e) {
                throw new IllegalStateException(String.format("Failed to join the annotations: " +
                                "annotations=%s, annotationsLength=%d, combined=%s, combinedReplacement=%s, " +
                                "combinedLength=%d, lengthAfterwards=%d, exception=%s",
                        Arrays.toString(annotations), annotationsLength, Arrays.toString(combined), Arrays.toString(combinedReplacement),
                        combinedLength, lengthAfterwards, e));
            }
            combined = combinedReplacement;
        }
        return combined;
    }

    /**
     * Returns the fields and parent fields of this class.
     *
     * @param clazz the class to be inspected.
     * @return the fields and parent fields of this class.
     */
    public static Field[] getCombinedFields(Class<?> clazz) {
        if (null == clazz)
            return new Field[0];
        Field[] fields = clazz.getDeclaredFields();
        Field[] parentFields = new Field[0];
        if (null != clazz.getSuperclass())
            parentFields = clazz.getSuperclass().getDeclaredFields();
        Field[] combined = new Field[fields.length + parentFields.length];
        System.arraycopy(fields, 0, combined, 0, fields.length);
        System.arraycopy(parentFields, 0, combined, fields.length, parentFields.length);
        return combined;
    }

    /**
     * Returns the methods and parent methods of this class.
     *
     * @param clazz the class to be inspected.
     * @return the methods and parent methods of this class.
     */
    public static Method[] getCombinedMethods(Class<?> clazz) {
        if (null == clazz)
            return new Method[0];
        Method[] methods = clazz.getDeclaredMethods();
        Method[] parentMethods = new Method[0];
        if (null != clazz.getSuperclass())
            parentMethods = clazz.getSuperclass().getDeclaredMethods();
        Method[] combined = new Method[methods.length + parentMethods.length];
        System.arraycopy(methods, 0, combined, 0, methods.length);
        System.arraycopy(parentMethods, 0, combined, methods.length, parentMethods.length);
        return combined;
    }

}
