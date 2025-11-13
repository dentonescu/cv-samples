package dev.dmotservices.tests.reflection;

import dev.dmotservices.reflection.Reflection;
import static org.junit.jupiter.api.Assertions.*;


import javax.inject.Inject;
import java.util.HashMap;
import java.util.Map;

public class ReflectionTests {

    private static final String IDENTIFIER = "A-0101-BX4J";
    private static final int INDEX_NUMBER = 340192;
    private static final String FIRST_NAME = "John";
    private static final String LAST_NAME = "Doe";

    public static class Member {

        @Inject
        private String identifier;
        @Inject
        private int indexNumber;
        @Inject
        private String firstName;
        @Inject
        private String lastName;

        public String getIdentifier() {
            return identifier;
        }

        public int getIndexNumber() {
            return indexNumber;
        }

        public String getFirstName() {
            return firstName;
        }

        public String getLastName() {
            return lastName;
        }

        @Override
        public String toString() {
            return "Member{" +
                    "identifier='" + identifier + '\'' +
                    ", indexNumber=" + indexNumber +
                    ", firstName='" + firstName + '\'' +
                    ", lastName='" + lastName + '\'' +
                    '}';
        }
    }

    private void injectOneByOne(Member member) {
        assertTrue(Reflection.injectValue(member, "identifier", IDENTIFIER));
        assertTrue(Reflection.injectValue(member, "indexNumber", INDEX_NUMBER));
        assertTrue(Reflection.injectValue(member, "firstName", FIRST_NAME));
        assertTrue(Reflection.injectValue(member, "lastName", LAST_NAME));
        checkBean(member);
    }

    private void massInject(Member member) {
        Map<String, Object> keyValueMap = new HashMap<>();
        keyValueMap.put("identifier", IDENTIFIER);
        keyValueMap.put("indexNumber", INDEX_NUMBER);
        keyValueMap.put("firstName", FIRST_NAME);
        keyValueMap.put("lastName", LAST_NAME);
        assertTrue(Reflection.massInject(member, keyValueMap));
        checkBean(member);
    }

    private void checkBean(Member member) {
        assertEquals(IDENTIFIER, member.getIdentifier());
        assertEquals(INDEX_NUMBER, member.getIndexNumber());
        assertEquals(FIRST_NAME, member.getFirstName());
        assertEquals(LAST_NAME, member.getLastName());
    }

    private void assertMembersMatch(Member expected, Member actual) {
        assertEquals(expected.getIdentifier(), actual.getIdentifier());
        assertEquals(expected.getIndexNumber(), actual.getIndexNumber());
        assertEquals(expected.getFirstName(), actual.getFirstName());
        assertEquals(expected.getLastName(), actual.getLastName());
    }

    @org.junit.jupiter.api.Test
    public void testInjection() {
        System.out.println("\n\ntestInjection():");
        Member member = new Member();
        System.out.format("member (before) => %s\n", member);
        injectOneByOne(member);
        System.out.format("member (after) => %s\n", member);
    }

    @org.junit.jupiter.api.Test
    public void testMassInjection() {
        System.out.println("\n\ntestMassInjection():");
        Member memberEmpty = new Member();
        Member memberIndividual = new Member();
        Member memberMass = new Member();
        injectOneByOne(memberIndividual);
        massInject(memberMass);
        System.out.format("memberEmpty = %s\n", memberEmpty);
        System.out.format("memberIndividual = %s\n", memberIndividual);
        System.out.format("memberMass = %s\n", memberMass);
        assertMembersMatch(memberIndividual, memberMass);
    }

}
