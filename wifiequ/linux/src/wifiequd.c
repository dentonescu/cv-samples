#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <microhttpd.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include "dmot/log.h"
#include "dmot/time.h"
#include "dmot/ui.h"
#include "wfq/wifiequ.h"
#include "wfq/config.h"
#include "wfq/mocksignal.h"
#include "wfq/wifisignal.h"

/*
 * internals
 */

static volatile sig_atomic_t g_running = 1;
static volatile sig_atomic_t g_reload = 0;
static wfq_options s_options;

static void on_signal(int sig)
{
    if (sig == SIGTERM || sig == SIGINT)
    {
        DMOT_LOGI("Stopping the daemon...");
        g_running = 0; // graceful stop
    }
    else if (sig == SIGHUP)
    {
        DMOT_LOGI("Reloading the configuration...");
        g_reload = 1; // reload config
    }
}

static void kernel_set_signal_handler(void)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_signal;
    sigemptyset(&sa.sa_mask); // block during handler
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
}

static void daemon_announce(FILE *out)
{
    DMOT_LOGI("Daemon started with PID %ld", (long)getpid());
    dmot_ui_ostream_repeat_pattern_endl(out, "=", 80);
    fputs("wifiequd\n", out);
    dmot_ui_ostream_repeat_pattern_endl(out, "-", 80);
    fprintf(out, "%-25s %s\n", WFQ_PARAM_MOCK, (s_options.mock ? "true" : "false"));
    fprintf(out, "%-25s %d\n", WFQ_PARAM_HTTP_PORT, s_options.port);
    dmot_ui_ostream_repeat_pattern_endl(out, "=", 80);
    fputs("\n", out);
}

static void set_up_signal_source(void)
{
    DMOT_LOGD("Setting up the signal source...");
    if (s_options.mock)
    {
        wfq_mock_signal_options o;
        o.n_channels = WFQ_EQU_N_CHANNELS;
        o.refresh_wait_ms = WFQ_MOCK_SIGNAL_REFRESH_WAIT_MS;
        wfq_sine_wave_generator_init_with_options(&o);
        wfq_sine_wave_generator_start();
        DMOT_LOGD("Started the (mock) sine wave signal generator.");
    }
    else
    {
        // TODO: set up live source
    }
}

static void tear_down_signal_source(void)
{
    DMOT_LOGD("Tearing down the signal source...");
    if (s_options.mock)
    {
        wfq_sine_wave_generator_stop();
    }
    else
    {
        // TODO: tear down live source
    }
}

static void publish_reading(void)
{
    static long long prev_timestamp = -1;
    char json[8192];
    wfq_sample sample = (s_options.mock ? wfq_sine_wave_generator_read() : wfq_wifi_signal_read());
    wfq_sample2json(&sample, json, sizeof json);
    if (sample.timestamp_ms > prev_timestamp)
    {
        DMOT_LOGD("%s", json);
    }
    // TODO: publish the reading to the HTTP stream
}

static void load_config(void)
{
    if (!wfq_config_read(&s_options)) {
        DMOT_LOGE("Failed to read the configuration file at: %s", WFQ_CONFIG_PATH);
        exit(EXIT_FAILURE);
    }
    DMOT_LOGD("Loaded the configuration.");
    g_reload = 0;
}

/*
 * externals
 */

void wfq_sample2json(wfq_sample *sample, char *buf, size_t buf_size)
{
    /*
     * Example:
     * {
     *   "timestamp_ms": 1739978905123,
     *   "channels_dbm": [-42.1, -36.5, -20.0, -12.3]
     *   }
     *
     */
    memset(buf, 0, buf_size);
    strncat(buf, "{", buf_size - strlen(buf) - 1);
    char tmp[64];
    snprintf(tmp, sizeof tmp, "\"timestamp_ms\": %lld, ", sample->timestamp_ms);
    strncat(buf, tmp, buf_size - strlen(buf) - 1);
    strncat(buf, "\"channels_dbm\": [", buf_size - strlen(buf) - 1);
    for (int i = 0; i < WFQ_EQU_N_CHANNELS; ++i)
    {
        snprintf(tmp, sizeof tmp, "%.2f", sample->channels_dbm[i]);
        strncat(buf, tmp, buf_size - strlen(buf) - 1);
        if (i < WFQ_EQU_N_CHANNELS - 1)
            strncat(buf, ", ", buf_size - strlen(buf) - 1);
    }
    strncat(buf, "]}", buf_size - strlen(buf) - 1);
}

int main(void)
{
    dmot_log_set_level(DMOT_LOG_DEBUG);
    dmot_log_set_file(stdout);
    load_config();
    kernel_set_signal_handler();
    daemon_announce(stdout);
    set_up_signal_source();
    while (g_running)
    {
        if (g_reload)
            load_config();
        publish_reading();
        dmot_time_sleep_ms(WFQ_REFRESH_WAIT_MS);
    }
    tear_down_signal_source();
    DMOT_LOGD("Daemon stopped.");
    return EXIT_SUCCESS;
}