package dev.dmotservices.heapmonj.dto;

import dev.dmotservices.system.HeapInfo;

/**
 * Serializable view of current heap metrics.
 */
public class HeapStatus {

    private long timestamp;
    private long heapSizeBytes;
    private long heapMaxBytes;
    private long heapFreeBytes;
    private double heapSizeMB;
    private double heapMaxMB;
    private double heapFreeMB;
    private float heapFreePercentage;

    public HeapStatus() {
    }

    /**
     * Capture a point-in-time snapshot of heap metrics.
     */
    public static HeapStatus now() {
        final long timestampNow = System.currentTimeMillis();
        final long sizeBytes = HeapInfo.getSizeBytes();
        final long maxBytes = HeapInfo.getMaxSizeBytes();
        final long freeBytes = HeapInfo.getFreeSizeBytes();

        final HeapStatus snapshot = new HeapStatus();
        snapshot.setTimestamp(timestampNow);
        snapshot.setHeapSizeBytes(sizeBytes);
        snapshot.setHeapMaxBytes(maxBytes);
        snapshot.setHeapFreeBytes(freeBytes);
        snapshot.setHeapSizeMB(sizeBytes / (1024.0 * 1024.0));
        snapshot.setHeapMaxMB(maxBytes / (1024.0 * 1024.0));
        snapshot.setHeapFreeMB(freeBytes / (1024.0 * 1024.0));
        snapshot.setHeapFreePercentage((freeBytes * 100.0f) / (float) sizeBytes);
        return snapshot;
    }

    /*
     * generated
     */

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    public long getHeapSizeBytes() {
        return heapSizeBytes;
    }

    public void setHeapSizeBytes(long heapSizeBytes) {
        this.heapSizeBytes = heapSizeBytes;
    }

    public long getHeapMaxBytes() {
        return heapMaxBytes;
    }

    public void setHeapMaxBytes(long heapMaxBytes) {
        this.heapMaxBytes = heapMaxBytes;
    }

    public long getHeapFreeBytes() {
        return heapFreeBytes;
    }

    public void setHeapFreeBytes(long heapFreeBytes) {
        this.heapFreeBytes = heapFreeBytes;
    }

    public double getHeapSizeMB() {
        return heapSizeMB;
    }

    public void setHeapSizeMB(double heapSizeMB) {
        this.heapSizeMB = heapSizeMB;
    }

    public double getHeapMaxMB() {
        return heapMaxMB;
    }

    public void setHeapMaxMB(double heapMaxMB) {
        this.heapMaxMB = heapMaxMB;
    }

    public double getHeapFreeMB() {
        return heapFreeMB;
    }

    public void setHeapFreeMB(double heapFreeMB) {
        this.heapFreeMB = heapFreeMB;
    }

    public float getHeapFreePercentage() {
        return heapFreePercentage;
    }

    public void setHeapFreePercentage(float heapFreePercentage) {
        this.heapFreePercentage = heapFreePercentage;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + (int) (timestamp ^ (timestamp >>> 32));
        result = prime * result + (int) (heapSizeBytes ^ (heapSizeBytes >>> 32));
        result = prime * result + (int) (heapMaxBytes ^ (heapMaxBytes >>> 32));
        result = prime * result + (int) (heapFreeBytes ^ (heapFreeBytes >>> 32));
        long temp;
        temp = Double.doubleToLongBits(heapSizeMB);
        result = prime * result + (int) (temp ^ (temp >>> 32));
        temp = Double.doubleToLongBits(heapMaxMB);
        result = prime * result + (int) (temp ^ (temp >>> 32));
        temp = Double.doubleToLongBits(heapFreeMB);
        result = prime * result + (int) (temp ^ (temp >>> 32));
        result = prime * result + Float.floatToIntBits(heapFreePercentage);
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null)
            return false;
        if (getClass() != obj.getClass())
            return false;
        HeapStatus other = (HeapStatus) obj;
        if (timestamp != other.timestamp)
            return false;
        if (heapSizeBytes != other.heapSizeBytes)
            return false;
        if (heapMaxBytes != other.heapMaxBytes)
            return false;
        if (heapFreeBytes != other.heapFreeBytes)
            return false;
        if (Double.doubleToLongBits(heapSizeMB) != Double.doubleToLongBits(other.heapSizeMB))
            return false;
        if (Double.doubleToLongBits(heapMaxMB) != Double.doubleToLongBits(other.heapMaxMB))
            return false;
        if (Double.doubleToLongBits(heapFreeMB) != Double.doubleToLongBits(other.heapFreeMB))
            return false;
        if (Float.floatToIntBits(heapFreePercentage) != Float.floatToIntBits(other.heapFreePercentage))
            return false;
        return true;
    }

    @Override
    public String toString() {
        return "HeapStatus [timestamp=" + timestamp + ", heapSizeBytes=" + heapSizeBytes + ", heapMaxBytes="
                + heapMaxBytes + ", heapFreeBytes=" + heapFreeBytes + ", heapSizeMB=" + heapSizeMB + ", heapMaxMB="
                + heapMaxMB + ", heapFreeMB=" + heapFreeMB + ", heapFreePercentage=" + heapFreePercentage + "]";
    }

}
