package dev.dmotservices.heapmonj;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.context.ApplicationContext;

import dev.dmotservices.heapmonj.dto.HeapStatus;

@SpringBootTest
class PojoTests {

	@Autowired
	private ApplicationContext applicationContext;

	@Test
	void testHeapStatusPojo() {
		assertNotNull(applicationContext);

		final HeapStatus heapStatus = HeapStatus.now();
		assertTrue(heapStatus.getTimestamp() > 0);
		assertTrue(heapStatus.getHeapFreeBytes() >= 0);
		assertTrue(heapStatus.getHeapFreeMB() >= 0);
		assertTrue(heapStatus.getHeapFreePercentage() >= 0);
		assertTrue(heapStatus.getHeapMaxBytes() > 0);
		assertTrue(heapStatus.getHeapMaxMB() > 0);
		assertTrue(heapStatus.getHeapSizeBytes() > 0);
		assertTrue(heapStatus.getHeapSizeMB() > 0);
	}

}
