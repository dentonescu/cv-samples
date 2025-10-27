#include <signal.h>
#include <stdarg.h>
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
#include "dmot/string.h"
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
static wfq_config_context s_config_ctx;

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
    fprintf(out, "%-25s %s\n", WFQ_PARAM_MOCK, (s_config_ctx.opt.mock ? "true" : "false"));
    fprintf(out, "%-25s %d\n", WFQ_PARAM_HTTP_PORT, s_config_ctx.opt.port);
    dmot_ui_ostream_repeat_pattern_endl(out, "=", 80);
    fputs("\n", out);
}

static void set_up_signal_source(void)
{
    DMOT_LOGD("Setting up the signal source...");
    if (s_config_ctx.opt.mock)
    {
        wfq_mock_signal_options o;
        o.n_channels = s_config_ctx.n_chan_defined;
        o.refresh_wait_ms = WFQ_MOCK_SIGNAL_REFRESH_WAIT_MS;
        wfq_sine_wave_generator_init_with_options(&o);
        wfq_sine_wave_generator_start();
        DMOT_LOGD("Started the (mock) sine wave signal generator.");
    }
    else
    {
        if (s_config_ctx.opt.interface[0] == '\0')
        {
            DMOT_LOGE("Live mode requires a configured interface (interface=...)");
            exit(EXIT_FAILURE);
        }
        wfq_wifi_signal_scanner_start(&s_config_ctx);
        DMOT_LOGD("Started the wi-fi scanner: interface=%s", s_config_ctx.opt.interface);
    }
}

static void tear_down_signal_source(void)
{
    DMOT_LOGD("Tearing down the signal source...");
    if (s_config_ctx.opt.mock)
    {
        wfq_sine_wave_generator_stop();
    }
    else
    {
        wfq_wifi_signal_scanner_stop();
    }
}

static void publish_reading(void)
{
    static long long prev_timestamp = -1;
    char json[8192];
    wfq_sample sample = (s_config_ctx.opt.mock ? wfq_sine_wave_generator_read() : wfq_wifi_signal_read());
    wfq_sample2json(&sample, json, sizeof json);
    if (sample.timestamp_ms > prev_timestamp)
    {
        DMOT_LOGD("%s", json);
    }
    // TODO: publish the reading to the HTTP stream
}

static void load_config(void)
{
    if (!wfq_config_read(&s_config_ctx))
    {
        DMOT_LOGE("Failed to read the configuration file at: %s", WFQ_CONFIG_PATH);
        exit(EXIT_FAILURE);
    }
    DMOT_LOGD("Loaded the configuration.");
    g_reload = 0;
}

static void reload_signal_source(void)
{
    DMOT_LOGI("Reloading signal source...");
    tear_down_signal_source();
    load_config();
    set_up_signal_source();
}

/*
 * externals
 */

void wfq_sample2json(wfq_sample *sample, char *buf, size_t buf_size)
{
    if (!buf || buf_size == 0)
        return;

    buf[0] = '\0';
    if (!sample)
        return;

    char *cursor = buf;
    char *end = buf + buf_size;

    if (!dmot_string_write_into(&cursor, end, "{\n"))
        return;
    if (!dmot_string_write_into(&cursor, end, "    \"timestamp_ms\": %lld,\n", sample->timestamp_ms))
        return;
    if (!dmot_string_write_into(&cursor, end, "    \"readings\": [\n"))
        return;

    bool first = true;
    for (size_t i = 0; i < WFQ_EQU_MAX_READINGS; ++i)
    {
        const wfq_channel *channel = &sample->readings[i];
        if (channel->chan_id <= 0)
            continue;

        if (!first && !dmot_string_write_into(&cursor, end, ",\n"))
            return;

        if (!dmot_string_write_into(&cursor, end,
                                    "        {\"chan\": %d, \"dbm\": %.1f}",
                                    channel->chan_id, channel->chan_dbm))
            return;

        first = false;
    }

    if (!dmot_string_write_into(&cursor, end, "\n    ]\n}\n"))
        return;
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
        {
            reload_signal_source();
            continue;
        }
        publish_reading();
        dmot_time_sleep_ms(WFQ_REFRESH_WAIT_MS);
    }
    tear_down_signal_source();
    DMOT_LOGD("Daemon stopped.");
    return EXIT_SUCCESS;
}
