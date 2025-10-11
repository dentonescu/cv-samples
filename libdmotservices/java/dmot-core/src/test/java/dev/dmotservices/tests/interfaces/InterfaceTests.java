package dev.dmotservices.tests.interfaces;

import dev.dmotservices.interfaces.impl.Base64ContentReader;
import static org.junit.jupiter.api.Assertions.*;


public class InterfaceTests {

    protected static final String base64Payload = "VGhpcyBCYXNlNjQgc3RyaW5nIHdhcyBzdWNjZXNmdWxseSBkZWNvZGVkIQ==";

    @org.junit.jupiter.api.Test
    public void testBase64Decode() {
        System.out.println("\ntestBase64Decode():");
        final Base64ContentReader content = new Base64ContentReader(base64Payload);
        final String original = new String(content.getOriginalRepresentation());
        final String decoded = new String(content.getContent());
        System.out.format("original representation = %s\n", original);
        System.out.format("decoded content = %s\n", decoded);
        assertEquals(base64Payload, original);
        assertEquals("This Base64 string was succesfully decoded!", decoded);
    }

}
