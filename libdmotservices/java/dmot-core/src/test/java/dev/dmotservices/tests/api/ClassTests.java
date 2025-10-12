package dev.dmotservices.tests.api;

import dev.dmotservices.io.ClassUtils;
import static org.junit.jupiter.api.Assertions.*;


import java.util.Set;

public class ClassTests {

    @org.junit.jupiter.api.Test
    public void testPackageLoader() {
        System.out.println("\n\ntestPackageLoader():");
        final Set<java.lang.Class<?>> mockSet = ClassUtils.findAllClassesInPackage("dev.dmotservices");
        System.out.println(mockSet);
        assertFalse(mockSet.isEmpty());
    }

}
