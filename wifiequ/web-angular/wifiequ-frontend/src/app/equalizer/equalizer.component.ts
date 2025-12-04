import { Component, OnInit, OnDestroy, ChangeDetectorRef } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Subject, takeUntil } from 'rxjs';
import { ChannelService } from '../services/channel.service'
import { EqualizerService } from '../services/equalizer.service';
import { Reading } from '../models/channels';
import { Stats } from '../models/stats';
import { ENDPOINTS } from '../api/endpoints';

@Component({
    selector: 'app-equalizer',
    standalone: true,
    imports: [CommonModule],
    templateUrl: './equalizer.html',
    styleUrl: './equalizer.scss'
})
export class EqualizerComponent implements OnInit, OnDestroy {

    endpoint: string = ENDPOINTS.channelsStream;
    error?: string;
    lastUpdated?: number;
    readings: Reading[] = [];
    stats?: Stats;

    private readonly destroy$ = new Subject<void>();

    constructor(
        private readonly channels: ChannelService,
        private readonly equalizer: EqualizerService,
        private readonly cdr: ChangeDetectorRef
    ) { }

    ngOnInit(): void {
    
        this.equalizer.getChannels()
            .pipe(takeUntil(this.destroy$))
            .subscribe({
                next: snapshotReceived => {
                    this.readings = this.normalizeReadings(snapshotReceived.readings);
                    this.lastUpdated = snapshotReceived.timestamp_ms;
                    this.cdr.detectChanges();
                },
                error: err => this.error = 'Failed to load the Wi-Fi channel data.',
            });
        this.equalizer.getStats()
            .pipe(takeUntil(this.destroy$))
            .subscribe({
                next: statsReceived => {
                    this.stats = statsReceived;
                    this.cdr.detectChanges();
                },
                error: err => this.error = 'Failed to load the WiFiEqu configuration.',
            });
        this.equalizer.getChannelStream()
            .pipe(takeUntil(this.destroy$))
            .subscribe({
                next: snapshotReceived => {
                    this.readings = this.normalizeReadings(snapshotReceived.readings);
                    this.lastUpdated = snapshotReceived.timestamp_ms;
                    this.cdr.detectChanges();
                },
                error: err => this.error = 'There was an error while processing the channel stream.'
            });

    }

    ngOnDestroy(): void {
        this.destroy$.next();
        this.destroy$.complete();
    }

    normalizeReadings(readings: Reading[]) {
        let distinctReadings: Reading[] = this.channels.getDistinctChannelReadings(readings);
        if (this.stats) {
            for (const bin of this.stats.channel_bins) {
                let reading = {} as Reading;
                reading.chan = bin.chan;
                reading.dbm = -100.0; // no signal
                if (!distinctReadings.some(r => r.chan === reading.chan)) {
                    distinctReadings.push(reading);
                }
            }
        }
        return this.channels.getSortedChannelReadings(distinctReadings);
    }

    // TrackBy: use channel number as the stable key so ngFor can reuse DOM nodes
    trackByChan(_: number, r: Reading) {
        return r.chan;
    }

    toPercent(dbm: number): number {
        const min = -90, max = -20; // tighter than -100 to 0 and gives a better visual spread
        const clamped = Math.min(Math.max(dbm, min), max);
        return ((clamped - min) / (max - min)) * 100;
    }

    signalStrengthClass(dbm: number): string {
        if (dbm <= -70) {
            return 'weak';
        }
        if (dbm <= -50) {
            return 'moderate';
        }
        if (dbm <= -30) {
            return 'strong';
        }
        return 'very-strong';
    }

}
