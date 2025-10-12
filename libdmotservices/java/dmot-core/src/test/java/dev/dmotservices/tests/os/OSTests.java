package dev.dmotservices.tests.os;

import dev.dmotservices.os.OS;
import static org.junit.jupiter.api.Assertions.*;


public class OSTests {

    @org.junit.jupiter.api.Test
    public void testGetLoggedInUser() {
        System.out.println("\n\ntestGetLoggedInUser():");
        final String loggedInUser = OS.getLoggedInUser();
        System.out.format("loggedInUser=%s\n", loggedInUser);
        assertTrue(null != loggedInUser && !loggedInUser.isBlank());
    }

}
