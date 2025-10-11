package dev.dmotservices.tests.io;

import dev.dmotservices.io.DataUtils;
import dev.dmotservices.io.FileUtils;
import org.json.JSONArray;
import static org.junit.jupiter.api.Assertions.*;


import java.io.IOException;

public class DataTests {

    protected static final String JSON;

    static {
        try {
            JSON = FileUtils.loadResourceAsString(DataTests.class, "json/payload.json");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    @org.junit.jupiter.api.Test
    public void testJsonIterator() {
        System.out.println("\n\ntestJsonIterator():");
        final JSONArray ja = new JSONArray(JSON);
        final int countKeyValuePairs = DataUtils.iterateOver(ja, (k, v) -> {
            System.out.format("k=%s, v=%s\n", k, v);
            assertFalse(String.valueOf(v).contains("{\""));
        });
        System.out.format("countKeyValuePairs=%d\n", countKeyValuePairs);
        assertTrue(countKeyValuePairs > 0);
    }

    @org.junit.jupiter.api.Test
    public void testContainsAllChars() {
        System.out.println("\n\ntestContainsAllChars():");
        final String testString = "This is a test!";
        final String testChars1 = "aes";
        final String testChars2 = "as+";
        final boolean test1 = DataUtils.containsAllChars(testString, testChars1);
        final boolean test2 = DataUtils.containsAllChars(testString, testChars2);
        System.out.format("testString=%s\n", testString);
        System.out.format("Contains testChars1=%s? [%s]\n", testChars1, test1);
        System.out.format("Contains testChars2=%s? [%s]\n", testChars2, test2);
        assertTrue(test1);
        assertFalse(test2);
    }
}
