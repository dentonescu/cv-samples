package dev.dmotservices.heapmonj;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.ActiveProfiles;

import dev.dmotservices.heapmonj.dto.HeapStatus;
import dev.dmotservices.heapmonj.entity.HeapSample;
import dev.dmotservices.heapmonj.repository.HeapSampleRepository;
import dev.dmotservices.heapmonj.service.HeapInfoService;

@SpringBootTest
@ActiveProfiles("test")
class HeapSampleRepositoryTests {

    @Autowired
    private HeapInfoService heapInfoService;

    @Autowired
    private HeapSampleRepository repository;

    @Test
    void saveAndRetrieveSnapshot() {
        final HeapStatus status = heapInfoService.currentSnapshot();
        final HeapSample toSave = heapInfoService.toSample(status);
        final HeapSample saved = repository.save(toSave);
        assertNotNull(saved.getId());
        final HeapSample found = repository.findById(saved.getId()).orElseThrow();
        assertEquals(saved.getTimestamp(), found.getTimestamp());
        assertEquals(saved.getHeapSizeBytes(), found.getHeapSizeBytes());
        assertEquals(saved.getHeapMaxBytes(), found.getHeapMaxBytes());
        assertEquals(saved.getHeapFreeBytes(), found.getHeapFreeBytes());
        assertEquals(saved.getHeapSizeMB(), found.getHeapSizeMB());
        assertEquals(saved.getHeapMaxMB(), found.getHeapMaxMB());
        assertEquals(saved.getHeapFreeMB(), found.getHeapFreeMB());
        assertEquals(saved.getHeapFreePercentage(), found.getHeapFreePercentage());
    }
}
