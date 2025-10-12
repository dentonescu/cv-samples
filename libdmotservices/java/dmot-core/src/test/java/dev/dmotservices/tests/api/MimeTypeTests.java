package dev.dmotservices.tests.api;

import dev.dmotservices.api.MimeType;
import static org.junit.jupiter.api.Assertions.*;


import java.util.Map;
import java.util.TreeMap;

public class MimeTypeTests {

    @org.junit.jupiter.api.Test
    public void testPartialMimeTypes() {
        System.out.println("\n\ntestPartialMimeTypes():");
        String[] partials = {"CSS", "doc", "DOCX", "gif", "htm", "jpg", "JPE", "js", "Xml"};
        for (String s : partials) {
            final MimeType mimeType = MimeType.fromPartialString(s);
            final String mimeTypeString = MimeType.extensionAsMimeTypeString(s);
            System.out.format("MimeType.fromPartialString: %s => %s\n", s, mimeType);
            System.out.format("MimeType.extensionAsMimeTypeString: %s => %s\n", s, mimeTypeString);
            assertNotNull(mimeType);
        }
    }

    @org.junit.jupiter.api.Test
    public void testGenerateMimeTypeList() {
        System.out.println("\n\ntestGenerateMimeTypeList()");
        final Map<String, String> mapExtMime = MimeType.fileExtensionMimeTypeStringMap();
        final Map<String, String> sortedMap = new TreeMap<>(mapExtMime);
        for (Map.Entry<String, String> entry : sortedMap.entrySet()) {
            String key = entry.getKey().toUpperCase()
                    .replaceAll("\\.", "")
                    .replaceAll("-", "_");
            String value = entry.getValue().toLowerCase();
            if (Character.isDigit(key.charAt(0)))
                key = "_" + key;
            System.out.format("%s(\"%s\"),\n", key, value);
        }
        System.out.println("\n\n");
    }

}
