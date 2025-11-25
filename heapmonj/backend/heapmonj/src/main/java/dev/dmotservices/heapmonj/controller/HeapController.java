package dev.dmotservices.heapmonj.controller;

import java.util.List;

import org.springframework.data.domain.PageRequest;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import dev.dmotservices.heapmonj.dto.HeapStatus;
import dev.dmotservices.heapmonj.entity.HeapSample;
import dev.dmotservices.heapmonj.repository.HeapSampleRepository;
import dev.dmotservices.heapmonj.service.HeapInfoService;

@RestController
@RequestMapping("/api/v1/heap")
public class HeapController {

    private final HeapInfoService heapInfoService;
    private final HeapSampleRepository repository;
    private static final int DEFAULT_LIMIT = 100;
    private static final int MAX_LIMIT = 1000;

    public HeapController(HeapInfoService heapInfoService, HeapSampleRepository repository) {
        this.heapInfoService = heapInfoService;
        this.repository = repository;
    }

    @GetMapping("/current")
    public HeapStatus current() {
        return heapInfoService.currentSnapshot();
    }

    @GetMapping("/history")
    public List<HeapSample> history(
            @RequestParam(value = "limit", required = false) Integer limit,
            @RequestParam(value = "since", required = false) Long since) {

        int effectiveLimit = Math.min(null == limit ? DEFAULT_LIMIT : Math.max(limit, 1), MAX_LIMIT);
        PageRequest page = PageRequest.of(0, effectiveLimit);
        return (null == since)
                ? repository.findAllByOrderByTimestampDesc(page)
                : repository.findByTimestampGreaterThanEqualOrderByTimestampDesc(since, page);
    }
}
