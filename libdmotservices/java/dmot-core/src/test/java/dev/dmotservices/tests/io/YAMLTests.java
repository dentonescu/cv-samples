package dev.dmotservices.tests.io;

import dev.dmotservices.api.yaml.YAMLContextPathFinder;
import static org.junit.jupiter.api.Assertions.*;


public class YAMLTests {

    protected static final String YAML_PATH = "resources/yaml/libdmotservices.yaml";

    @org.junit.jupiter.api.Test
    public void testYAMLContextPathFinder() {
        System.out.println("\n\ntestYAMLContextPathFinder():");
        YAMLContextPathFinder yamlContextPathFinder = new YAMLContextPathFinder(YAML_PATH);
        System.out.format("yamlContextPathFinder=%s\n", yamlContextPathFinder);
        assertEquals("/alpha", yamlContextPathFinder.findContextPathOfMethod("sayAlpha"));
        assertEquals("/bravo", yamlContextPathFinder.findContextPathOfMethod("sayBravo"));
        assertEquals("/charlie", yamlContextPathFinder.findContextPathOfMethod("sayCharlie"));
    }

}
