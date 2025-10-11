package dev.dmotservices.io;

import io.github.classgraph.ClassGraph;
import io.github.classgraph.ClassInfo;
import io.github.classgraph.ScanResult;

import java.util.Set;
import java.util.stream.Collectors;

public class ClassUtils {
    public static Set<Class<?>> findAllClassesInPackage(String packageName) {
        if (packageName == null || packageName.isBlank())
            throw new IllegalArgumentException("The package name must not be empty.");
        try (ScanResult scan = new ClassGraph()
                .acceptPackages(packageName)
                .enableClassInfo()
                .scan()) {
            return scan.getAllClasses().stream()
                    .filter(ci -> !ci.getName().contains("$")) // skip inner/anonymous classes
                    .map(ClassInfo::loadClass)
                    .collect(Collectors.toSet());
        }
    }

    private static java.lang.Class<?> getClassInPackage(String className, String packageName) {
        if (null == className)
            throw new IllegalArgumentException("The class name cannot be empty.");
        if (null == packageName)
            throw new IllegalArgumentException("The package name cannot be empty.");
        try {
            return java.lang.Class.forName(packageName + "."
                    + className.substring(0, className.lastIndexOf('.')));
        } catch (ClassNotFoundException e) {
            throw new IllegalStateException(String.format("Could not find the class %s in package %s.\n%s", className, packageName, e));
        }
    }
}
