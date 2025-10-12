package dev.dmotservices.tests.converters;

import dev.dmotservices.converters.UTF8StringCodec;
import static org.junit.jupiter.api.Assertions.*;


import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class CharsetTests {

    @org.junit.jupiter.api.Test
    public void garbledStringConversionTest() {
        System.out.println("\ngarbledStringConversionTest()");
        String test = "t├╝rkisch";
        String processed = new UTF8StringCodec().process(test);
        System.out.println(processed);
        assertEquals("türkisch", processed);
    }

    @org.junit.jupiter.api.Test
    public void specialCharsTest() {
        System.out.println("\nspecialCharsTest()");
        byte[] a = new String("ä".getBytes(), StandardCharsets.UTF_8).getBytes();
        byte[] A = new String("Ä".getBytes(), StandardCharsets.UTF_8).getBytes();
        byte[] o = new String("ö".getBytes(), StandardCharsets.UTF_8).getBytes();
        byte[] O = new String("Ö".getBytes(), StandardCharsets.UTF_8).getBytes();
        byte[] u = new String("ü".getBytes(), StandardCharsets.UTF_8).getBytes();
        byte[] U = new String("Ü".getBytes(), StandardCharsets.UTF_8).getBytes();
        byte[] sz = new String("ß".getBytes(), StandardCharsets.UTF_8).getBytes();

        System.out.println("ä = " + Arrays.toString(a));
        System.out.println("Ä = " + Arrays.toString(A));
        System.out.println("ö = " + Arrays.toString(o));
        System.out.println("Ö = " + Arrays.toString(O));
        System.out.println("ü = " + Arrays.toString(u));
        System.out.println("Ü = " + Arrays.toString(U));
        System.out.println("ß = " + Arrays.toString(sz));
    }

    @org.junit.jupiter.api.Test
    public void UTF8toASCIItoUTF8Test() {
        System.out.println("\nUTF8toASCIItoUTF8Test()");
        String utf8 = new String("Härte Ägypten Föhn Öl üben Übung aß".getBytes(), StandardCharsets.UTF_8);
        System.out.println("utf8 = " + utf8);
        String iso_8859_1 = new String(utf8.getBytes(StandardCharsets.UTF_8), StandardCharsets.ISO_8859_1);
        System.out.println("iso_8859_1 = " + iso_8859_1);
        String utf8_prime = new String(iso_8859_1.getBytes(StandardCharsets.ISO_8859_1), StandardCharsets.UTF_8);
        System.out.println("utf8_prime = " + utf8_prime);
        assertEquals(utf8, utf8_prime);
    }

}
