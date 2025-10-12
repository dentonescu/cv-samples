package dev.dmotservices.api;

public enum RequestMethod {
    DELETE("DELETE"), GET("GET"), HEAD("HEAD"), OPTIONS("OPTIONS"), PATCH("PATCH"), POST("POST"), PUT("PUT"), TRACE("TRACE");

    private final String requestMethod;

    RequestMethod(String requestMethod) {
        this.requestMethod = requestMethod;
    }

    public String getRequestMethod() {
        return requestMethod;
    }

    public static RequestMethod fromString(String method) {
        if (null == method)
            return null;
        final String trimmed = method.trim();
        for (RequestMethod requestMethod : RequestMethod.values()) {
            if (requestMethod.getRequestMethod().equalsIgnoreCase(trimmed)) {
                return requestMethod;
            }
        }
        return null;
    }
}
