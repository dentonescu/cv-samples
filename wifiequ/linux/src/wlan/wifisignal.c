#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "dmot/log.h"
#include "dmot/math.h"
#include "dmot/time.h"
#include "dmot/ui.h"
#include "wfq/config.h"
#include "wfq/wifisignal.h"
#include "wfq/wifiequ.h"
#include "wfq/wlanscan.h"

/*
 * internals
 */

static pthread_t t_thread;
static volatile bool g_running = false;
static pthread_mutex_t sample_mx = PTHREAD_MUTEX_INITIALIZER;
static wfq_sample sample = {0};
static long refresh_wait_ms = WFQ_LIVE_SIGNAL_REFRESH_WAIT_MS;

static void *wfq_task_wifi_poll(void *arg)
{
    wfq_config_context *ctx = (wfq_config_context *)arg;
    DMOT_LOGD("wfq_task_wifi_poll: iface=%s", ctx->opt.interface);
    while (g_running)
    {
        wfq_sample tmp = {0};
        wfq_signal *signals = wfq_scan_wlan(ctx->opt.interface);
        size_t readings_written = 0;
        bool truncated = false;
        size_t scan_index = 0;
        for (; scan_index < WFQ_WLAN_SCAN_MAX_RESULTS; ++scan_index)
        {
            wfq_signal sig = signals[scan_index];
            if (sig.freq_mhz <= 0.0)
                break;
            if (readings_written >= WFQ_EQU_MAX_READINGS)
            {
                truncated = true;
                break;
            }
            const int chan_wifi = wfq_config_freq2chan(ctx, sig.freq_mhz);
            tmp.readings[readings_written].chan_id = chan_wifi;
            tmp.readings[readings_written].chan_dbm = sig.strength_dbm;
            ++readings_written;
        }
        bool more_signals = (scan_index == WFQ_WLAN_SCAN_MAX_RESULTS && signals[scan_index].freq_mhz > 0.0);
        if (truncated || more_signals)
        {
            DMOT_LOGW("wfq_task_wifi_poll: truncated live readings (written=%zu, max=%d, more_signals=%s)",
                      readings_written, WFQ_EQU_MAX_READINGS, more_signals ? "yes" : "no");
        }
        tmp.timestamp_ms = dmot_time_now_ms();
        pthread_mutex_lock(&sample_mx);
        sample = tmp; // avoid data race while writing
        pthread_mutex_unlock(&sample_mx);
        dmot_time_sleep_ms(refresh_wait_ms);
    }
    return NULL;
}

/*
 * externals
 */

wfq_sample wfq_wifi_signal_read(void)
{
    wfq_sample out;
    pthread_mutex_lock(&sample_mx);
    out = sample; // avoid data race while reading
    pthread_mutex_unlock(&sample_mx);
    return out;
}

void wfq_wifi_signal_scanner_start(wfq_config_context *ctx)
{
    if (!g_running)
    {
        g_running = true;
        pthread_create(&t_thread, NULL, wfq_task_wifi_poll, ctx);
    }
}

void wfq_wifi_signal_scanner_stop(void)
{
    if (g_running)
    {
        g_running = false;
        pthread_join(t_thread, NULL);
    }
}
