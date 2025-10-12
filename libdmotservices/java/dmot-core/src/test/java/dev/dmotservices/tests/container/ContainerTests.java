package dev.dmotservices.tests.container;

import dev.dmotservices.container.ContainerUtils;


public class ContainerTests {

    public static final String CONTAINER_CONFIG_PATH = "../mock/tomcat/conf/catalina.properties";

    @org.junit.jupiter.api.Test
    public void testProcessContainerConfig() {
        System.out.println("\n\ntestProcessContainerConfig():");
        ContainerUtils.processContainerConfig(CONTAINER_CONFIG_PATH);
    }

}
