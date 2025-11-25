package dev.dmotservices.heapmonj.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import dev.dmotservices.heapmonj.entity.HeapSample;

import java.util.List;

/**
 * Spring Data derives the implementation from method names; no manual
 * implementation required.
 */
@Repository
public interface HeapSampleRepository extends JpaRepository<HeapSample, Long> {

    // SELECT * FROM heap_samples ORDER BY timestamp DESC LIMIT ?
    List<HeapSample> findAllByOrderByTimestampDesc(org.springframework.data.domain.Pageable pageable);

    // SELECT * FROM heap_samples WHERE timestamp >= ? ORDER BY timestamp DESC LIMIT ?
    List<HeapSample> findByTimestampGreaterThanEqualOrderByTimestampDesc(long timestamp,
            org.springframework.data.domain.Pageable pageable);
}
