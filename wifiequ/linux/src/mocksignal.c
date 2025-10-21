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
#include "wfq/mocksignal.h"
#include "wfq/wifiequ.h"

static pthread_t t_thread;
static volatile bool g_running = false;
static wfq_sample sample = {0};
static int n_channels = WFQ_EQU_N_CHANNELS;
static long refresh_wait_ms = WFQ_MOCK_SIGNAL_REFRESH_WAIT_MS;

void *wfq_task_sine_wave_generator(void *arg)
{
    (void)arg;
    while (g_running)
    {
        for (int chan = 1; chan <= n_channels; ++chan)
        {
            sample.channels_dbm[chan - 1] = dmot_mathx_rand_double(DMOT_UI_EQU_DBM_LOWEST, DMOT_UI_EQU_DBM_HIGHEST);
        }
        sample.timestamp_ms = timestamp_ms();
        sleep_ms(refresh_wait_ms);
    }
}

void wfq_sine_wave_generator_init(void)
{
    wfq_sine_wave_generator_stop();
    refresh_wait_ms = WFQ_MOCK_SIGNAL_REFRESH_WAIT_MS;
    for (int chan = 1; chan <= n_channels; ++chan)
    {
        sample.channels_dbm[chan - 1] = DMOT_UI_EQU_DBM_LOWEST;
    }
    sample.timestamp_ms = timestamp_ms();
}

void wfq_sine_wave_generator_init_with_options(wfq_mock_signal_options *options)
{
    if (!options)
    {
        DMOT_LOGE("Empty options suplplied to the sine wave signal generator.\n");
        return;
    }
    wfq_sine_wave_generator_stop();
    for (int chan = 1; chan <= n_channels; ++chan)
    {
        sample.channels_dbm[chan - 1] = DMOT_UI_EQU_DBM_LOWEST;
    }
    refresh_wait_ms = options->refresh_wait_ms;
    sample.timestamp_ms = timestamp_ms();
}

wfq_sample wfq_sine_wave_generator_read(void)
{
    return sample;
}

void wfq_sine_wave_generator_start(void)
{
    if (!g_running)
    {
        g_running = true;
        pthread_create(&t_thread, NULL, wfq_task_sine_wave_generator, NULL);
    }
}

void wfq_sine_wave_generator_stop(void)
{
    if (g_running)
    {
        g_running = false;
        pthread_join(t_thread, NULL);
    }
}