
package dev.dmotservices.versioning;

public class Versioning {

    private static final String VERSION = "0.3.0";
    private static final String BASE_VERSION = VERSION.split("-", 2)[0];
    private static final String[] VERSION_PARTS = BASE_VERSION.split("\\.");

    public static String getVersion() {
        return VERSION;
    }

    public static int getMajor() {
        return Integer.parseInt(VERSION_PARTS[0]);
    }

    public static int getMinor() {
        return (VERSION_PARTS.length > 1) ? Integer.parseInt(VERSION_PARTS[1]) : 0;
    }

    public static int getPatch() {
        return (VERSION_PARTS.length > 2) ? Integer.parseInt(VERSION_PARTS[2]) : 0;
    }
}
