package dev.dmotservices.tests.system;

import dev.dmotservices.system.HeapInfo;
import static org.junit.jupiter.api.Assertions.*;


public class SystemTests {

    private static final long TOLERANCE = 1;

    @org.junit.jupiter.api.Test
    public void testHeapBytes() {
        System.out.println("\n\ntestHeapBytes():");
        long maxSizeBytes = HeapInfo.getMaxSizeBytes();
        long sizeBytes = HeapInfo.getSizeBytes();
        long freeSizeBytes = HeapInfo.getFreeSizeBytes();
        float freePercentage = HeapInfo.getFreePercentage();
        System.out.format("Max: %d\n", maxSizeBytes);
        System.out.format("Heap: %d\n", sizeBytes);
        System.out.format("Free: %d\n", freeSizeBytes);
        System.out.format("Free %%: %f\n", freePercentage);
        assertTrue(freeSizeBytes >= 0 &&
                maxSizeBytes >= 0 &&
                sizeBytes >= 0 &&
                (freeSizeBytes * 100) / (float) sizeBytes + TOLERANCE >= freePercentage);
    }

    @org.junit.jupiter.api.Test
    public void testHeapMB() {
        System.out.println("\n\ntestHeapMB():");
        double maxSizeMB = HeapInfo.getMaxSizeMB();
        double sizeMB = HeapInfo.getSizeMB();
        double freeSizeMB = HeapInfo.getFreeSizeMB();
        float freePercentage = HeapInfo.getFreePercentage();
        System.out.format("Max: %f\n", maxSizeMB);
        System.out.format("Heap: %f\n", sizeMB);
        System.out.format("Free: %f\n", freeSizeMB);
        System.out.format("Free %%: %f\n", freePercentage);
        assertTrue(freeSizeMB >= 0 &&
                maxSizeMB >= 0 &&
                sizeMB >= 0 &&
                (freeSizeMB * 100) / (float) sizeMB + TOLERANCE >= freePercentage);
    }

    @org.junit.jupiter.api.Test
    public void testHeapGB() {
        System.out.println("\n\ntestHeapGB():");
        double maxSizeGB = HeapInfo.getMaxSizeGB();
        double sizeGB = HeapInfo.getSizeGB();
        double freeSizeGB = HeapInfo.getFreeSizeGB();
        float freePercentage = HeapInfo.getFreePercentage();
        System.out.format("Max: %f\n", maxSizeGB);
        System.out.format("Heap: %f\n", sizeGB);
        System.out.format("Free: %f\n", freeSizeGB);
        System.out.format("Free %%: %f\n", freePercentage);
        assertTrue(freeSizeGB >= 0 &&
                maxSizeGB >= 0 &&
                sizeGB >= 0 &&
                (freeSizeGB * 100) / (float) sizeGB + TOLERANCE >= freePercentage);
    }

    @org.junit.jupiter.api.Test
    public void testJsonObject() {
        System.out.println("\n\ntestJsonObject():");
        final String json = HeapInfo.toJson().toString();
        System.out.println(json);
        assertTrue(json.startsWith("{") &&
                json.endsWith("}"));
    }

}
