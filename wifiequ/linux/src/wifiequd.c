#define _GNU_SOURCE
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
#include "dmot/timex.h"
#include "dmot/ui.h"
#include "wfq/wifiequ.h"
#include "wfq/mocksignal.h"
#include "wfq/wifisignal.h"

/*
 * internals
 */

static volatile sig_atomic_t g_running = 1;
static volatile sig_atomic_t g_reload = 0;
static wfq_options s_options;

static void help(FILE *out)
{
    dmot_ui_ostream_repeat_pattern_endl(out, "=", 80);
    fputs("wifiequd: Help\n", out);
    dmot_ui_ostream_repeat_pattern_endl(out, "=", 80);
    fprintf(out, "%-25s %s\n", "--mock", "Uses a fake sine wave signal as the input source.");
    fprintf(out, "%-25s %s\n", "--port [port_number]", "e.g. 8080");
    fputs("\n", out);
}

static void on_signal(int sig)
{
    if (sig == SIGTERM || sig == SIGINT)
    {
        DMOT_LOGI("Stopping the daemon...\n");
        g_running = 0; // graceful stop
    }
    else if (sig == SIGHUP)
    {
        DMOT_LOGI("Reloading the configuration...\n");
        g_reload = 1; // reload config
    }
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
        DMOT_LOGD("Started the (mock) sine wave signal generator.\n");
    }
    else
    {
        // TODO: set up live source
    }
}

static void tear_down_signal_source(void)
{
    DMOT_LOGD("Tearing down the signal source...\n");
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
        DMOT_LOGD("%s\n", json);
    }
    // TODO: publish the reading to the HTTP stream
}

static void reload_config(void)
{
    set_up_signal_source();
    // TODO: (re)load configuration here.
    DMOT_LOGD("Reloaded configuration.\n");
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

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        help(stdout);
        return EXIT_SUCCESS;
    }
    s_options.port = -1;
    s_options.mock = false;
    bool misunderstood = false;

    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "--port") && i + 1 < argc)
            s_options.port = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--mock"))
            s_options.mock = true;
        else
            misunderstood = true;
    }

    if (misunderstood)
    {
        DMOT_LOGE("A parameter was supplied as an argument which was not understood.\n");
        help(stdout);
        return EXIT_FAILURE;
    }

    dmot_log_set_level(DMOT_LOG_DEBUG);
    dmot_log_set_file(stdout);

    // instruct the kernel to use the on_signal handler when the signals listed below arrive
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_signal;
    sigemptyset(&sa.sa_mask); // block during handler
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);

    DMOT_LOGI("Daemon started with PID %ld\n", (long)getpid());
    DMOT_LOGD("Options: mock=%s, port=%d\n",
              (s_options.mock ? "true" : "false"),
              s_options.port);

    set_up_signal_source();
    while (g_running)
    {
        if (g_reload)
            reload_config();
        publish_reading();
        sleep_ms(WFQ_REFRESH_WAIT_MS);
    }
    tear_down_signal_source();

    DMOT_LOGD("Daemon stopped.\n");
    return EXIT_SUCCESS;
}