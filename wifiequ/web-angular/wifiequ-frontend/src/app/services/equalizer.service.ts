import { Injectable } from '@angular/core';
import { HttpClient, HttpParams } from '@angular/common/http';
import { Observable } from 'rxjs';

import { Channels } from '../models/channels';
import { Stats } from '../models/stats';
import { ENDPOINTS } from '../api/endpoints';

@Injectable({
    providedIn: 'root'
})
export class EqualizerService {
    constructor(private http: HttpClient) { }

    getChannelStream(): Observable<Channels> {
        return new Observable<Channels>((observer) => {
            const es = new EventSource(ENDPOINTS.channelsStream);
            const handler = (event: MessageEvent) => {
                if (!event.data || !event.data.trim()) {
                    return; // ignore empty/heartbeat payloads
                }
                try {
                    observer.next(JSON.parse(event.data) as Channels);
                } catch (err) {
                    console.warn('Skipping bad SSE payload', event.data, err);
                }
            };
            es.addEventListener('reading', handler);
            es.onerror = (err) => observer.error(err);
            return () => es.close(); // RxJS expects a teardown function. This anonymous function simply closes the EventSource.
        });
    }

    getChannels(): Observable<Channels> {
        return this.http.get<Channels>(ENDPOINTS.channels);
    }

    getStats(): Observable<Stats> {
        return this.http.get<Stats>(ENDPOINTS.stats);
    }
}