package dev.dmotservices.container;

import dev.dmotservices.pkix.PKIX;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Properties;

public class ContainerUtils {

    public static final String DEFAULT_CONTAINER_CONFIG_FILE_NAME = "catalina.properties";

    public static final String PROPERTY_CATALINA_BASE = "catalina.base";

    public static final String PROPERTY_PKIX_VALIDATION_ENABLED = "pkix.validation.enabled";

    public static String getContainerBasePath() throws FileNotFoundException {
        final String catalinaBasePath = System.getProperty(PROPERTY_CATALINA_BASE);
        if (null == catalinaBasePath || catalinaBasePath.isBlank())
            throw new FileNotFoundException(String.format("Could not ascertain Tomcat's main directory -- %s.", PROPERTY_CATALINA_BASE));
        return catalinaBasePath;
    }

    public static String getContainerConfigPath() throws FileNotFoundException {
        final String[] pathNames = {getContainerBasePath(), "conf", "catalina.properties"};
        return String.join(File.separator, pathNames);
    }

    public static Properties getContainerConfigProperties() {
        final Properties properties = new Properties();
        try {
            properties.load(new FileInputStream(getContainerConfigPath()));
        } catch (IOException e) {
            return null;
        }
        return properties;
    }

    public static void processContainerConfig() {
        try {
            processContainerConfig(getContainerConfigPath());
        } catch (FileNotFoundException e) {
            throw new IllegalStateException(String.format("Failed to determine the path to %s.\n%s",
                    DEFAULT_CONTAINER_CONFIG_FILE_NAME, e));
        }
    }

    public static void processContainerConfig(String containerConfigPath) {
        if (null == containerConfigPath || containerConfigPath.isBlank())
            throw new IllegalArgumentException(String.format("The path to the container's configuration file (e.g., %s) must be specified.",
                    DEFAULT_CONTAINER_CONFIG_FILE_NAME));
        final Properties properties = new Properties();
        try {
            properties.load(new FileInputStream(containerConfigPath));
        } catch (IOException e) {
            throw new IllegalStateException(String.format("Failed to load the properties file: %s\n%s", containerConfigPath, e));
        }
        final String pkixValidationEnabled = (String) properties.get(PROPERTY_PKIX_VALIDATION_ENABLED);
        final boolean isPKIXValidation = (null == pkixValidationEnabled || !pkixValidationEnabled.trim().equalsIgnoreCase("false"));
        if (!isPKIXValidation) {
            // turn off PKIX validation
            PKIX.disableCertificateValidation();
        }
    }

}
