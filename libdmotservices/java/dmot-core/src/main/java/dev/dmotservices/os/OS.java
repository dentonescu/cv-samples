package dev.dmotservices.os;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.logging.Logger;

public class OS {

    private static final Logger logger = Logger.getLogger(OS.class.getName());

    public static String getLoggedInUser() {
        String loggedInUser = null;
        if (System.getProperty("os.name").toLowerCase().contains("win")) {
            logger.fine("Using 'query user' to determine the logged-in Windows user.");
            try {
                // We try "query user" first because Windows will return the Tomcat service account otherwise.
                final Process process = Runtime.getRuntime().exec("query user");
                String line;
                try (BufferedReader reader = new BufferedReader(
                        new InputStreamReader(process.getInputStream())
                )) {
                    while ((line = reader.readLine()) != null) {
                        if (line.toLowerCase().contains("activ") || line.toLowerCase().contains("aktiv")) {
                            // This will not work for languages which don't look like this
                            String[] parts = line.split("\\s+");
                            loggedInUser = parts[0];
                            break;
                        }
                    }
                }
            } catch (IOException e) {
                logger.warning("Failed to get the current user with 'query user'. Trying alternatives.");
            }
        }
        if (null == loggedInUser || loggedInUser.isBlank()) {
            loggedInUser = System.getenv("USERNAME");
            if (null == loggedInUser || loggedInUser.isBlank()) {
                loggedInUser = System.getenv("USER");
                if (null == loggedInUser || loggedInUser.isBlank()) {
                    loggedInUser = System.getProperty("user.name");
                }
            }
        }
        if (null != loggedInUser)
            loggedInUser = loggedInUser.trim();
        return loggedInUser;
    }

}
