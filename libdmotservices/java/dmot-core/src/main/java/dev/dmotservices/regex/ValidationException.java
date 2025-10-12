package dev.dmotservices.regex;

@SuppressWarnings("unused")
public class ValidationException extends Exception {
    public ValidationException(String errorMessage) {
        super(errorMessage);
    }
    public ValidationException(String errorMessage, Throwable err) {
        super(errorMessage, err);
    }
}
