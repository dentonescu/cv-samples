package dev.dmotservices.api.annotations;

import java.lang.annotation.*;

@Target({ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface OperationId {
    String name() default "";
    String value() default "";
}