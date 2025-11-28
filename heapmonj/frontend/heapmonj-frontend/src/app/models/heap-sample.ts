import { HeapStatus } from './heap-status';

export interface HeapSample extends HeapStatus {
  id?: number;
}
