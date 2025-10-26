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
#include "wfq/mocksignal.h"
#include "wfq/wifiequ.h"

/*
 * internals
 */

static pthread_t t_thread;
static volatile bool g_running = false;
static pthread_mutex_t sample_mx = PTHREAD_MUTEX_INITIALIZER;
static wfq_sample sample = {0};
static int n_channels = WFQ_EQU_N_CHANNELS;
static long refresh_wait_ms = WFQ_MOCK_SIGNAL_REFRESH_WAIT_MS;

/*
 * externals
 */

void *wfq_task_sine_wave_generator(void *arg)
{
    (void)arg;
    wfq_sample tmp;
    while (g_running)
    {
        for (int chan = 1; chan <= n_channels; ++chan)
        {
            tmp.channels_dbm[chan - 1] = dmot_math_rand_double(DMOT_UI_EQU_DBM_LOWEST, DMOT_UI_EQU_DBM_HIGHEST);
        }
        tmp.timestamp_ms = dmot_time_now_ms();
        pthread_mutex_lock(&sample_mx);
        sample = tmp; // avoid data race while writing
        pthread_mutex_unlock(&sample_mx);
        dmot_time_sleep_ms(refresh_wait_ms);
    }
    return NULL;
}

void wfq_sine_wave_generator_init(void)
{
    wfq_sine_wave_generator_stop();
    refresh_wait_ms = WFQ_MOCK_SIGNAL_REFRESH_WAIT_MS;
    for (int chan = 1; chan <= n_channels; ++chan)
    {
        sample.channels_dbm[chan - 1] = DMOT_UI_EQU_DBM_LOWEST;
    }
    sample.timestamp_ms = dmot_time_now_ms();
}

void wfq_sine_wave_generator_init_with_options(wfq_mock_signal_options *options)
{
    if (!options)
    {
        DMOT_LOGE("Empty options supplied to the sine wave signal generator.");
        return;
    }
    wfq_sine_wave_generator_stop();
    int requested = options->n_channels;
    if (requested < 1)
        requested = 1;
    if (requested > WFQ_EQU_N_CHANNELS)
        requested = WFQ_EQU_N_CHANNELS;
    n_channels = requested;
    for (int chan = 1; chan <= requested; ++chan)
    {
        sample.channels_dbm[chan - 1] = DMOT_UI_EQU_DBM_LOWEST;
    }
    refresh_wait_ms = options->refresh_wait_ms;
    sample.timestamp_ms = dmot_time_now_ms();
}

wfq_sample wfq_sine_wave_generator_read(void)
{
    wfq_sample out;
    pthread_mutex_lock(&sample_mx);
    out = sample; // avoid data race while reading
    pthread_mutex_unlock(&sample_mx);
    return out;
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