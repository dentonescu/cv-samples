
package dev.dmotservices.versioning;

/**
 * Provides access to the current libdmotservices version at runtime.
 */
public class Versioning {

    private static final String VERSION = "0.3.0";
    private static final String BASE_VERSION = VERSION.split("-", 2)[0];
    private static final String[] VERSION_PARTS = BASE_VERSION.split("\\.");

    /**
     * Returns the full semantic version string.
     *
     * @return The raw version string, including any pre-release suffix.
     */
    public static String getVersion() {
        return VERSION;
    }

    /**
     * Returns the major component of the version.
     *
     * @return Major version number.
     */
    public static int getMajor() {
        return Integer.parseInt(VERSION_PARTS[0]);
    }

    /**
     * Returns the minor component of the version.
     *
     * @return Minor version number, or {@code 0} when absent.
     */
    public static int getMinor() {
        return (VERSION_PARTS.length > 1) ? Integer.parseInt(VERSION_PARTS[1]) : 0;
    }

    /**
     * Returns the patch component of the version.
     *
     * @return Patch version number, or {@code 0} when absent.
     */
    public static int getPatch() {
        return (VERSION_PARTS.length > 2) ? Integer.parseInt(VERSION_PARTS[2]) : 0;
    }
}
