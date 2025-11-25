package dev.dmotservices.heapmonj.entity;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Table;

/**
 * Persisted heap snapshot.
 */
@Entity
@Table(name = "heap_samples")
public class HeapSample {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private long timestamp;
    private long heapSizeBytes;
    private long heapMaxBytes;
    private long heapFreeBytes;
    private double heapSizeMB;
    private double heapMaxMB;
    private double heapFreeMB;
    private float heapFreePercentage;

    public HeapSample() {
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

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
}
