package dev.dmotservices.reflection;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

public interface AnnotationHandler {
    void process(Field field);
    void process(Method method);
}
