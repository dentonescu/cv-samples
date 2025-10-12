package dev.dmotservices.io;

import java.util.HashMap;
import java.util.Map;

public class UriUtils {

    public static String getLastPathSegement(java.net.URI uri) {
        if (null == uri)
            throw new IllegalArgumentException("URI cannot be empty.");
        return getLastPathSegement(uri.getPath());
    }

    public static String getLastPathSegement(String uri) {
        if (uri.endsWith("/"))
            uri = uri.substring(0, uri.length() - 1);
        return uri.substring(uri.lastIndexOf('/') + 1);
    }

    public static Map<String, String> getQueryMap(String query) {
        final Map<String, String> map = new HashMap<>();
        if (null == query || query.isBlank())
            return map;
        String[] params = query.split("&");
        for (String param : params) {
            String name = param.split("=")[0];
            String value = param.split("=")[1];
            map.put(name, value);
        }
        return map;
    }

    public static java.net.URI concatenate(Object... elements) {
        // TODO: This definitely needs to be improved or swapped out for something else. Later.
        StringBuilder sbUri = new StringBuilder();
        for (Object element : elements) {
            if (null != element) {
                if (sbUri.toString().contains("://") &&
                        !sbUri.toString().endsWith("/") &&
                        !sbUri.toString().contains("?") &&
                        !element.toString().startsWith("/") &&
                        !element.toString().startsWith("?") &&
                        !element.toString().startsWith("#")) {
                    sbUri.append("/");
                }
                sbUri.append(element);
            }
        }
        return java.net.URI.create(sbUri.toString());
    }

}
