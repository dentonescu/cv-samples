package dev.dmotservices.tests.security;

import dev.dmotservices.security.Security;
import static org.junit.jupiter.api.Assertions.*;

public class SecurityTests {

    @org.junit.jupiter.api.Test
    public void testGeneratePassword() {
        System.out.println("\n\ntestGeneratePassword():");
        final String password1 = Security.generatePassword();
        final String password2 = Security.generatePassword(8);
        final String password3 = Security.generatePassword(32, ".-");
        System.out.format("password1=%s\n", password1);
        System.out.format("password2=%s\n", password2);
        System.out.format("password3=%s\n", password3);
    }

    @SuppressWarnings({"ThrowablePrintedToSystemOut", "ConstantValue"})
    @org.junit.jupiter.api.Test
    public void testGeneratePasswordWithInvalidValues() {
        System.out.println("\n\ntestGeneratePasswordWithInvalidValues():");
        boolean successTest1 = false;
        boolean successTest2 = false;
        boolean successTest3 = false;
        System.out.println("Will supply invalid arguments in three cases:");
        try {
            Security.generatePassword(-1);
        } catch (IllegalArgumentException e) {
            successTest1 = true;
            System.err.println(e);
        }
        try {
            Security.generatePassword(16, "");
        } catch (IllegalArgumentException e) {
            successTest2 = true;
            System.err.println(e);
        }
        try {
            Security.generatePassword(32, null);
        } catch (IllegalArgumentException e) {
            successTest3 = true;
            System.err.println(e);
        }
        if (successTest1 && successTest2 && successTest3) {
            System.out.println("The errors were caught and exceptions raised.");
        } else
            throw new IllegalStateException("An invalid argument was not caught. Please inspect the password generator.");
    }

    @org.junit.jupiter.api.Test
    public void testRequiredPasswordChars() {
        System.out.println("\n\ntestRequiredPasswordChars()");
        final String mustContain = "$3X";
        System.out.format("mustContain=%s\n", mustContain);
        for (int i = 0; i < 10; ++i) {
            final String password = Security.generatePassword(32, Security.DEFAULT_PASSWORD_ALLOWED_CHARACTERS, mustContain);
            System.out.println(password);
            for (int j = 0; j < mustContain.length(); ++j) {
                final char c = mustContain.charAt(j);
                assertTrue(password.indexOf(c) >= 0);
            }
        }
    }

    @org.junit.jupiter.api.Test
    public void testEncryptDecryptUsingSecretFile() {
        System.out.println("\n\ntestEncryptDecryptUsingSecretFile():");
        Security.SecretFile secretFile = new Security.SecretFile();
        secretFile.setFilename("src/test/resources/secret/payload.secret-demo");
        secretFile.setEncryptionAlgorithm("AES");
        final String payload = "The text to be encrypted.";
        System.out.format("payload=%s\n", payload);
        final String encryptedString = Security.encryptValueUsingKeyFile(secretFile, payload);
        System.out.format("encryptedString=%s\n", encryptedString);
        final String decryptedString = Security.decryptValueUsingKeyFile(secretFile, encryptedString);
        System.out.format("decryptedString=%s\n", decryptedString);
        assertTrue(null != encryptedString && !encryptedString.isBlank());
        assertTrue(null != decryptedString && !decryptedString.isBlank());
        assertEquals(payload, decryptedString);
    }

}
