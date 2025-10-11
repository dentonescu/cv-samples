package dev.dmotservices.reflection;

import java.lang.reflect.Field;

public interface FieldChecker {
    boolean isRequiredField(Field field);
}