import { Component, OnDestroy, OnInit, ViewChild, ChangeDetectorRef } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Subject, forkJoin, of, timer } from 'rxjs';
import { catchError, switchMap, takeUntil } from 'rxjs/operators';
import { ChartConfiguration, ChartOptions } from 'chart.js';
import { BaseChartDirective } from 'ng2-charts';

import { HeapmonService } from '../services/heapmon.service';
import { HeapStatus } from '../models/heap-status';
import { HeapSample } from '../models/heap-sample';
import { APP_CONFIG } from '../config';


@Component({
  selector: 'app-heapmon',
  standalone: true,
  imports: [CommonModule, BaseChartDirective],
  templateUrl: './heapmon.html',
  styleUrl: './heapmon.scss',
})
export class HeapmonComponent implements OnInit, OnDestroy {

  @ViewChild(BaseChartDirective) private chart?: BaseChartDirective;

  protected current?: HeapStatus;
  protected lastUpdated?: Date;
  protected error?: string;

  protected chartData: ChartConfiguration<'line'>['data'] = {
    labels: [],
    datasets: [
      {
        label: 'Used (MB)',
        data: [],
        fill: true,
        borderColor: '#a855f7',
        backgroundColor: 'rgba(168, 85, 247, 0.15)',
        tension: 0.25
      },
      {
        label: 'Free (MB)',
        data: [],
        fill: true,
        borderColor: '#22d3ee',
        backgroundColor: 'rgba(34, 211, 238, 0.15)',
        tension: 0.25
      }
    ]
  };

  protected readonly chartOptions: ChartOptions<'line'> = {
    responsive: true,
    maintainAspectRatio: false,
    animation: false,
    plugins: {
      legend: {
        labels: { color: '#e5e7eb' }
      }
    },
    scales: {
      x: {
        ticks: { color: '#cbd5e1', maxRotation: 0 },
        grid: { display: false }
      },
      y: {
        ticks: { color: '#cbd5e1' },
        grid: { color: 'rgba(148, 163, 184, 0.1)' },
        title: { display: true, text: 'MB', color: '#cbd5e1' }
      }
    }
  };

  protected readonly pollIntervalMs = APP_CONFIG.pollIntervalMs;
  private readonly destroy$ = new Subject<void>();

  constructor(
    private readonly heapmon: HeapmonService,
    private readonly cdr: ChangeDetectorRef
  ) { }

  ngOnInit(): void {
    this.startPolling();
  }

  ngOnDestroy(): void {
    this.destroy$.next();
    this.destroy$.complete();
  }

  private startPolling(): void {
    timer(0, this.pollIntervalMs).pipe(
      switchMap(() => forkJoin({
        current: this.heapmon.getCurrent(),
        history: this.heapmon.getHistory(APP_CONFIG.historyLimit)
      })),
      takeUntil(this.destroy$),
      catchError(err => {
        const msg = err?.message || 'Unable to reach backend.';
        this.error = msg;
        // eslint-disable-next-line no-console
        console.error('Heapmon polling error', err);
        return of(null);
      })
    ).subscribe(result => {
      if (!result) {
        this.error = 'No data received from backend.';
        return;
      }
      // eslint-disable-next-line no-console
      this.error = undefined;
      const fallback = result.history?.length ? result.history[result.history.length - 1] : undefined;
      this.current = result.current ?? fallback;
      if (this.current?.timestamp) {
        this.lastUpdated = new Date(this.current.timestamp);
      }
      this.updateChart(result.history);
    });
  }

  private updateChart(samples: HeapSample[]): void {
    const sorted = [...samples]
      .sort((a, b) => a.timestamp - b.timestamp)
      .slice(-APP_CONFIG.historyLimit);
    const labels = sorted.map(s => new Date(s.timestamp).toLocaleTimeString());
    const used = sorted.map(s => s.heapSizeMB - s.heapFreeMB);
    const free = sorted.map(s => s.heapFreeMB);

    this.chartData = {
      labels,
      datasets: [
        { ...this.chartData.datasets[0], data: used },
        { ...this.chartData.datasets[1], data: free }
      ]
    };
    // Ensure the chart redraws even if change detection misses the update.
    this.chart?.update();
    this.cdr.detectChanges();
  }
}
