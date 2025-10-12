package dev.dmotservices.tests.generators;

import dev.dmotservices.generators.SequentialIdGenerator;
import static org.junit.jupiter.api.Assertions.*;


public class GeneratorTests {

    @org.junit.jupiter.api.Test
    public void testSequentialIdGenerator() {
        System.out.println("\ntestSequentialIdGenerator():");
        long previousId = 554;
        System.out.format("Starting at %d ...\n", previousId + 1);
        SequentialIdGenerator.init(previousId);
        for (int i = 1; i <= 10; ++i) {
            long id = SequentialIdGenerator.nextNumeric();
            System.out.format("SequentialIdGenerator.nextNumeric() = %d\n", id);
            assertTrue(id > previousId);
            previousId = id;
        }
    }

}
