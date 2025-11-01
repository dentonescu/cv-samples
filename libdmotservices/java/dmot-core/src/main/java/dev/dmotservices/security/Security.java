package dev.dmotservices.security;

import dev.dmotservices.io.DataUtils;

import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.security.SecureRandom;
import java.util.Base64;
import java.util.Objects;
import java.util.Random;

public class Security {

    public static final int DEFAULT_PASSWORD_LENGTH = 16;
    public static final String DEFAULT_PASSWORD_ALLOWED_CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz$-/.^&*_!@%=+>)";

    public static String generatePassword() {
        return generatePassword(DEFAULT_PASSWORD_LENGTH);
    }

    public static String generatePassword(int length) {
        return generatePassword(length, DEFAULT_PASSWORD_ALLOWED_CHARACTERS);
    }

    public static String generatePassword(int length, String allowedCharacters) {
        return generatePassword(length, allowedCharacters, null);
    }

    public static String generatePassword(int length, String allowedCharacters, String mustContain) {
        if (null == allowedCharacters || allowedCharacters.isBlank())
            throw new IllegalArgumentException("The characters which constitute the password must be specified.");
        if (length < 1)
            throw new IllegalArgumentException("A password must contain at least one character.");
        StringBuilder sbPassword;
        do {
            sbPassword = new StringBuilder();
            final Random random = new SecureRandom();
            for (int i = 0; i < length; ++i) {
                final int charIndex = Math.abs(random.nextInt()) % (allowedCharacters.length());
                sbPassword.append(allowedCharacters.charAt(charIndex));
            }
        } while (null != mustContain && !DataUtils.containsAllChars(sbPassword.toString(), mustContain));
        return sbPassword.toString();
    }

    public static class SecretFile {
        String filename;
        String encryptionAlgorithm;

        /*
         * generated
         */

        public String getEncryptionAlgorithm() {
            return encryptionAlgorithm;
        }

        public void setEncryptionAlgorithm(String encryptionAlgorithm) {
            this.encryptionAlgorithm = encryptionAlgorithm;
        }

        public String getFilename() {
            return filename;
        }

        public void setFilename(String filename) {
            this.filename = filename;
        }

        @Override
        public boolean equals(Object o) {
            if (o == null || getClass() != o.getClass()) return false;
            SecretFile keyFile = (SecretFile) o;
            return Objects.equals(filename, keyFile.filename) && Objects.equals(encryptionAlgorithm, keyFile.encryptionAlgorithm);
        }

        @Override
        public int hashCode() {
            return Objects.hash(filename, encryptionAlgorithm);
        }

        @Override
        public String toString() {
            return "KeyFile{" +
                    "filename='" + filename + '\'' +
                    ", encryptionAlgorithm='" + encryptionAlgorithm + '\'' +
                    '}';
        }
    }

    public static String encryptValueUsingKeyFile(SecretFile secretFile, String payload) {
        if (null == secretFile || null == secretFile.getFilename() || secretFile.getFilename().isBlank() ||
                null == secretFile.getEncryptionAlgorithm() || secretFile.getEncryptionAlgorithm().isBlank())
            throw new IllegalArgumentException("A complete SecretFile object must be supplied.");
        if (null == payload || payload.isBlank())
            return payload;
        try (FileInputStream fileIn = new FileInputStream(secretFile.getFilename());
             ObjectInputStream objectIn = new ObjectInputStream(fileIn)) {
            final SecretKey secretKey = (SecretKey) objectIn.readObject();
            final Cipher cipher = Cipher.getInstance(secretFile.getEncryptionAlgorithm());
            cipher.init(Cipher.ENCRYPT_MODE, secretKey);
            final byte[] encryptedData = cipher.doFinal(payload.getBytes());
            return Base64.getEncoder().encodeToString(encryptedData);
        } catch (Exception e) {
            throw new IllegalStateException(String.format("FAILED to encrypt the string using the secret key in %s.\n%s",
                    secretFile.getFilename(), e));
        }
    }

    public static String decryptValueUsingKeyFile(SecretFile secretFile, String payload) {
        if (null == secretFile || null == secretFile.getFilename() || secretFile.getFilename().isBlank() ||
                null == secretFile.getEncryptionAlgorithm() || secretFile.getEncryptionAlgorithm().isBlank())
            throw new IllegalArgumentException("A complete SecretFile object must be supplied.");
        if (null == payload || payload.isBlank())
            return payload;
        try (FileInputStream fileIn = new FileInputStream(secretFile.getFilename());
             ObjectInputStream objectIn = new ObjectInputStream(fileIn)) {
            final SecretKey secretKey = (SecretKey) objectIn.readObject();
            final Cipher cipher = Cipher.getInstance(secretFile.getEncryptionAlgorithm());
            cipher.init(Cipher.DECRYPT_MODE, secretKey);
            final byte[] encryptedData = Base64.getDecoder().decode(payload);
            byte[] decryptedData = cipher.doFinal(encryptedData);
            return new String(decryptedData);
        } catch (Exception e) {
            throw new IllegalStateException(String.format("FAILED to decrypt the string using the secret key in %s.\n%s",
                    secretFile.getFilename(), e));
        }
    }

}
