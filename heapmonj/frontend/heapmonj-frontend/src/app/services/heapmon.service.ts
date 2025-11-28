import { Injectable } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { Observable } from 'rxjs';

import { HeapStatus } from '../models/heap-status';
import { HeapSample } from '../models/heap-sample';
import { ENDPOINTS } from '../api/endpoints';

@Injectable({
  providedIn: 'root'
})
export class HeapmonService {

  constructor(private http: HttpClient) { }

  getCurrent(): Observable<HeapStatus> {
    return this.http.get<HeapStatus>(ENDPOINTS.current);
  }

  getHistory(limit?: number, since?: number): Observable<HeapSample[]> {
    let params = new HttpParams();
    if (limit !== undefined) {
      params = params.set('limit', limit.toString());
    }
    if (since !== undefined) {
      params = params.set('since', since.toString());
    }
    return this.http.get<HeapSample[]>(ENDPOINTS.history, { params });
  }
}
