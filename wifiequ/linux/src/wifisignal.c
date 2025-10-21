#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "dmot/log.h"
#include "dmot/mathx.h"
#include "dmot/timex.h"
#include "dmot/ui.h"
#include "wfq/wifisignal.h"
#include "wfq/wifiequ.h"

static pthread_t t_thread;
static volatile bool g_running = false;
static wfq_sample sample = {0};
static int n_channels = WFQ_EQU_N_CHANNELS;
static long refresh_wait_ms = WFQ_LIVE_SIGNAL_REFRESH_WAIT_MS;

void *wfq_task_wifi_poll(void *arg)
{
    (void)arg;
    while (g_running)
    {
        for (int chan = 1; chan <= n_channels; ++chan)
        {
            sample.channels_dbm[chan - 1] = DMOT_UI_EQU_DBM_LOWEST; // TODO: use an actual reading
        }
        sample.timestamp_ms = timestamp_ms();
        sleep_ms(refresh_wait_ms);
    }
}

wfq_sample wfq_wifi_signal_read(void)
{
    return sample;
}