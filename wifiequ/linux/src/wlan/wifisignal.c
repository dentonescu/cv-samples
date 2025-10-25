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
#include "wfq/wifisignal.h"
#include "wfq/wifiequ.h"

static pthread_t t_thread;
static volatile bool g_running = false;
static pthread_mutex_t sample_mx = PTHREAD_MUTEX_INITIALIZER;
static wfq_sample sample = {0};
static int n_channels = WFQ_EQU_N_CHANNELS;
static long refresh_wait_ms = WFQ_LIVE_SIGNAL_REFRESH_WAIT_MS;

void *wfq_task_wifi_poll(void *arg)
{
    (void)arg;
    wfq_sample tmp;
    while (g_running)
    {
        for (int chan = 1; chan <= n_channels; ++chan)
        {
            tmp.channels_dbm[chan - 1] = DMOT_UI_EQU_DBM_LOWEST; // TODO: use an actual reading
        }
        tmp.timestamp_ms = dmot_time_now_ms();
        pthread_mutex_lock(&sample_mx);
        sample = tmp; // avoid data race while writing
        pthread_mutex_unlock(&sample_mx);
        dmot_time_sleep_ms(refresh_wait_ms);
    }
}

wfq_sample wfq_wifi_signal_read(void)
{
    wfq_sample out;
    pthread_mutex_lock(&sample_mx);
    out = sample; // avoid data race while reading
    pthread_mutex_unlock(&sample_mx);
    return out;
}