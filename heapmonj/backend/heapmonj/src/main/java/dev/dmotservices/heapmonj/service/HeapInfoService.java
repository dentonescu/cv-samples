package dev.dmotservices.heapmonj.service;

import org.springframework.stereotype.Service;

import dev.dmotservices.heapmonj.dto.HeapStatus;
import dev.dmotservices.heapmonj.entity.HeapSample;

@Service
public class HeapInfoService {

    /**
     * Capture the current heap status.
     */
    public HeapStatus currentSnapshot() {
        return HeapStatus.now();
    }

    /**
     * Map a HeapStatus DTO to a JPA entity.
     */
    public HeapSample toSample(HeapStatus status) {
        HeapSample sample = new HeapSample();
        sample.setTimestamp(status.getTimestamp());
        sample.setHeapSizeBytes(status.getHeapSizeBytes());
        sample.setHeapMaxBytes(status.getHeapMaxBytes());
        sample.setHeapFreeBytes(status.getHeapFreeBytes());
        sample.setHeapSizeMB(status.getHeapSizeMB());
        sample.setHeapMaxMB(status.getHeapMaxMB());
        sample.setHeapFreeMB(status.getHeapFreeMB());
        sample.setHeapFreePercentage(status.getHeapFreePercentage());
        return sample;
    }
}
