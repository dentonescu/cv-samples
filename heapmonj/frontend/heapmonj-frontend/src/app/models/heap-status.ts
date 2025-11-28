export interface HeapStatus {
  timestamp: number;
  heapSizeBytes: number;
  heapMaxBytes: number;
  heapFreeBytes: number;
  heapSizeMB: number;
  heapMaxMB: number;
  heapFreeMB: number;
  heapFreePercentage: number;
}
