package dev.dmotservices.tests.versioning;

import dev.dmotservices.versioning.Versioning;
import org.junit.jupiter.api.Assertions;


public class VersionTests {

    @org.junit.jupiter.api.Test
    public void testVersion() {
        System.out.println("\n\ntestVersion():");
        final String version = Versioning.getVersion();
        System.out.format("version=%s\n", version);
        final int major = Versioning.getMajor();
        final int minor = Versioning.getMinor();
        final int patch = Versioning.getPatch();
        System.out.format("major=%d, minor=%d, patch=%d\n", major, minor, patch);
        final String numericVersion = version.split("-", 2)[0];
        final String reconstituted = "" + major + '.' + minor + '.' + patch;
        Assertions.assertEquals(numericVersion, reconstituted);
    }

}
