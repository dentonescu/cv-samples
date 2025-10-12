package dev.dmotservices.io;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

public class DataUtils {

    public interface Mapper {
        void process(Object k, Object v);
    }

    /**
     * Checks if testString contains all the characters given by chars.
     *
     * @param testString a string to be checked against a list of characters.
     * @param chars      a string containing characters which ought to be in the test string.
     * @return true if all the characters of chars are to be found in testString.
     */
    public static boolean containsAllChars(String testString, String chars) {
        if (null == testString || null == chars || testString.isBlank() || chars.isBlank())
            throw new IllegalArgumentException("The supplied parameters cannot be empty.");
        final Set<Character> testStringSet = new HashSet<>();
        final Set<Character> charsSet = new HashSet<>();
        for (char c : testString.toCharArray()) {
            testStringSet.add(c);
        }
        for (char c : chars.toCharArray()) {
            charsSet.add(c);
        }
        return testStringSet.containsAll(charsSet);
    }

    /**
     * Walks through each nested item in an object, identifies key-value pairs and presents them to the mapper for it to take action.
     *
     * @param o      an object which possibly contains nested items.
     * @param mapper an implementation of a mapper which does something with the key-value pairs.
     * @return a count of the number of key-value pairs found.
     */
    public static int iterateOver(Object o, Mapper mapper) {
        return iterateOver(o, mapper, null);
    }

    protected static int iterateOver(Object o, Mapper mapper, String inputKey) {
        if (null == o)
            throw new IllegalArgumentException("The supplied object cannot be empty.");
        if (null == mapper)
            throw new IllegalArgumentException("The mapper cannot be empty.");
        int countKeyValuePairs = 0;
        Iterator<?> keys;
        if (o instanceof JSONObject) {
            keys = ((JSONObject) o).keys();
            while (keys.hasNext()) {
                final String key = (String) keys.next();
                final Object value = ((JSONObject) o).get(key);
                if (value instanceof JSONObject ||
                        value instanceof JSONArray) {
                    countKeyValuePairs += iterateOver(value, mapper, key);
                } else {
                    mapper.process(key, value);
                    ++countKeyValuePairs;
                }
            }
        } else if (o instanceof JSONArray ja) {
            for (int i = 0; i < ja.length(); ++i) {
                final Object nestedObject = ja.get(i);
                if (nestedObject instanceof JSONObject ||
                        nestedObject instanceof JSONArray) {
                    countKeyValuePairs += iterateOver(nestedObject, mapper);
                } else {
                    /* The nested object is an array of single items and not of key-value pairs.
                     * Here's what this type of key-value pair looks like. inputKey, at this point, contains "employeeId".
                     * nestedObject contains 37343438
                     *  "employeeId": [
                     *     "37343438"
                     * ]
                     */
                    mapper.process(inputKey, nestedObject);
                }
            }
        } else {
            throw new IllegalArgumentException(
                    String.format("An automatic way of acquiring the keys for an object of type %s has not been implemented.",
                            o.getClass().getName()));
        }
        return countKeyValuePairs;
    }

    public static String shortenTextMiddle(String s) {
        return shortenTextMiddle(s, 35, 35);
    }

    public static String shortenTextMiddle(String s, int prefixLength, int suffixLength) {
        if (null == s)
            return null;
        final String spacer = " [...] ";
        final int len = s.length();
        final int truncationThreshold = prefixLength + suffixLength + spacer.length();
        if (len <= truncationThreshold)
            return s;
        final String start = s.substring(0, prefixLength);
        final String end = s.substring(len - suffixLength);
        return start + spacer + end;
    }

}
