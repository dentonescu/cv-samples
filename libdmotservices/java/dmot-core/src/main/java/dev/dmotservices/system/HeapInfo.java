package dev.dmotservices.system;

import org.json.JSONObject;

public class HeapInfo {

    public static long getSizeBytes() {
        return Runtime.getRuntime().totalMemory();
    }

    public static double getSizeMB() {
        return getSizeBytes() / (1024.0 * 1024.0);
    }

    public static double getSizeGB() {
        return getSizeBytes() / (1024.0 * 1024.0 * 1024.0);
    }

    public static long getMaxSizeBytes() {
        return Runtime.getRuntime().maxMemory();
    }

    public static double getMaxSizeMB() {
        return getMaxSizeBytes() / (1024.0 * 1024.0);
    }

    public static double getMaxSizeGB() {
        return getMaxSizeBytes() / (1024.0 * 1024 * 1024.0);
    }

    public static long getFreeSizeBytes() {
        return Runtime.getRuntime().freeMemory();
    }

    public static double getFreeSizeMB() {
        return getFreeSizeBytes() / (1024.0 * 1024.0);
    }

    public static double getFreeSizeGB() {
        return getFreeSizeBytes() / (1024.0 * 1024.0 * 1024.0);
    }

    public static float getFreePercentage() {
        return (getFreeSizeBytes() * 100) / (float) getSizeBytes();
    }

    public static JSONObject toJson() {
        JSONObject jo = new JSONObject();
        jo.put("heapFreeByes", getFreeSizeBytes());
        jo.put("heapFreeMB", getFreeSizeMB());
        jo.put("heapFreeGB", getFreeSizeGB());
        jo.put("heapFreePercentage", getFreePercentage());
        jo.put("heapSizeBytes", getSizeBytes());
        jo.put("heapSizeMB", getSizeMB());
        jo.put("heapSizeGB", getSizeGB());
        jo.put("heapSizeMaxBytes", getMaxSizeBytes());
        jo.put("heapSizeMaxMB", getMaxSizeMB());
        jo.put("heapSizeMaxGB", getMaxSizeGB());
        return jo;
    }
}
