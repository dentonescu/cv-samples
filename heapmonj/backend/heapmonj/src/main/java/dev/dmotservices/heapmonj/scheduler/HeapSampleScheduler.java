package dev.dmotservices.heapmonj.scheduler;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import dev.dmotservices.heapmonj.repository.HeapSampleRepository;
import dev.dmotservices.heapmonj.service.HeapInfoService;

@Component
public class HeapSampleScheduler {

    private static final Logger log = LoggerFactory.getLogger(HeapSampleScheduler.class);

    private final HeapInfoService heapInfoService;
    private final HeapSampleRepository repository;

    public HeapSampleScheduler(HeapInfoService heapInfoService, HeapSampleRepository repository) {
        this.heapInfoService = heapInfoService;
        this.repository = repository;
    }

    @Scheduled(fixedRateString = "${heapmonj.sampling.interval-ms:10000}")
    public void captureAndStore() {
        var sample = repository.save(heapInfoService.toSample(heapInfoService.currentSnapshot()));
        log.debug("Recorded heap sample id={} ts={} usedMB={} freeMB={} maxMB={}",
                sample.getId(),
                sample.getTimestamp(),
                sample.getHeapSizeMB(),
                sample.getHeapFreeMB(),
                sample.getHeapMaxMB());
    }
}
