package dev.dmotservices.tests.regex;

import dev.dmotservices.regex.StandardValidation;
import dev.dmotservices.regex.Validator;
import static org.junit.jupiter.api.Assertions.*;


public class RegexTests {

    @SuppressWarnings("ConstantValue")
    @org.junit.jupiter.api.Test
    public void testBlank() {
        System.out.println("\n\ntestBlank()");
        final String s = "         ";
        final boolean resultBlank = Validator.isSafeString(s);
        final boolean resultNull = Validator.isSafeString(null);
        System.out.format("%s -> %s\n", s, resultBlank);
        System.out.format("NULL -> %s\n", resultNull);
        assertTrue(resultBlank);
        assertTrue(resultNull);
    }

    @org.junit.jupiter.api.Test
    public void testRestrictedSQL() {
        System.out.println("\n\ntestRestrictedSQL()");
        final StandardValidation validation = StandardValidation.RESTRICTED_SQL;
        final String without = "a day of hard work";
        final String withApostrophe = "a hard day's work";
        final String withSemicolon = ";DROP DATABASE evil;";
        final String test1 = "Fachpraktiker/in für Landwirt (§66 BBiG/§42r HwO)";
        final boolean resultWithout = Validator.isSafeString(without, validation);
        final boolean resultWithApostrophe = Validator.isSafeString(withApostrophe, validation);
        final boolean resultWithSemicolon = Validator.isSafeString(withSemicolon, validation);
        final boolean resultTest1 = Validator.isSafeString(test1, validation);
        System.out.format("%s -> %s\n", without, resultWithout);
        System.out.format("%s -> %s\n", withApostrophe, resultWithApostrophe);
        System.out.format("%s -> %s\n", withSemicolon, resultWithSemicolon);
        System.out.format("%s -> %s\n", test1, resultTest1);
        assertTrue(resultWithout);
        assertTrue(resultWithApostrophe);
        assertFalse(resultWithSemicolon);
        assertTrue(resultTest1);
    }

    @org.junit.jupiter.api.Test
    public void testStandard() {
        System.out.println("\n\ntestStandard()");
        final String s = "A stich in time saves nine.";
        final boolean result = Validator.isSafeString(s);
        System.out.format("%s -> %s\n", s, result);
        assertTrue(result);
    }

    @org.junit.jupiter.api.Test
    public void testWesternNames() {
        System.out.println("\n\ntestWesternNames()");
        final String[] examples = {"Max Mustermann", "Ines Gühring", "Vlad Țepeș",
                "Sean O'Connor", "'; DROP DATABASE fakedatabase"};
        final boolean[] results = new boolean[examples.length];
        final boolean[] expectedResults = {true, true, true, true, false};
        for (int i = 0; i < examples.length; ++i) {
            results[i] = Validator.isSafeString(examples[i], StandardValidation.WESTERN_TEXT);
            System.out.format("examples[%d] (isSafeString=%s): %s\n", i, results[i], examples[i]);
            assertEquals(expectedResults[i], results[i]);
        }
    }

    @org.junit.jupiter.api.Test
    public void testWholeNumbers() {
        System.out.println("\n\ntestWholeNumbers()");
        final String[] examples = {"123456", "4221A"};
        final boolean[] results = new boolean[examples.length];
        final boolean[] expectedResults = {true, false};
        for (int i = 0; i < examples.length; ++i) {
            results[i] = Validator.isSafeString(examples[i], StandardValidation.WHOLE_NUMBER);
            System.out.format("examples[%d] (isSafeString=%s): %s\n", i, results[i], examples[i]);
            assertEquals(expectedResults[i], results[i]);
        }
    }

}
