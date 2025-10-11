package dev.dmotservices.regex;

public class Validator {

    public static boolean isSafeString(String s) {
        return isSafeString(s, StandardValidation.WESTERN_TEXT);
    }

    public static boolean isSafeString(String s, StandardValidation standardValidation) {
        return isSafeString(s, standardValidation.getRegex());
    }

    public static boolean isSafeString(String s, String regex) {
        // If the string is empty, we consider it safe.
        if (null == s || s.trim().isBlank())
            return true;
        return s.matches(regex);
    }

}
