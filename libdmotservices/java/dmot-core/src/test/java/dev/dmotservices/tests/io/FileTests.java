package dev.dmotservices.tests.io;

import dev.dmotservices.io.DataUtils;
import dev.dmotservices.io.FileUtils;
import static org.junit.jupiter.api.Assertions.*;


import java.io.IOException;
import java.nio.charset.StandardCharsets;

public class FileTests {

    public static final String TEST_RESOURCE = "resources/txt/loremipsum.txt";

    @org.junit.jupiter.api.Test
    public void testLoadLocalResourceAsString() throws IOException {
        System.out.println("\n\ntestLoadLocalResourceAsString():");
        final String connectionPropertiesString = FileUtils.loadResourceAsString(getClass(), TEST_RESOURCE);
        assertFalse(connectionPropertiesString.isEmpty());
        System.out.println("length of connection properties = " + connectionPropertiesString.length());
    }

    @org.junit.jupiter.api.Test
    public void testLoadFromJar() throws IOException {
        System.out.println("\n\ntestLoadFromJar():");
        final boolean readFromJarOnly = true;
        final String loremIpsumString = FileUtils.loadLocalResourceAsString(getClass(), TEST_RESOURCE, StandardCharsets.UTF_8, readFromJarOnly);
        System.out.println(loremIpsumString);
        assertFalse(loremIpsumString.isEmpty());
    }

    @org.junit.jupiter.api.Test
    public void testLoadNetworkResourceAsString() throws IOException {
        final String url = "https://www.google.com/robots.txt";
        System.out.format("\n\ntestLoadNetworkResourceAsString(URL=%s)\n", url);
        final String payload = FileUtils.loadResourceAsString(getClass(), url);
        System.out.println("Payload (shortened):");
        System.out.println(DataUtils.shortenTextMiddle(payload));
        System.out.println("Statistics:");
        System.out.format("payload length = %d", payload.length());
        assertTrue(payload.contains("User-agent"));
        assertTrue(payload.contains("Sitemap:"));
    }

}
