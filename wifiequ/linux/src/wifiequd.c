#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>
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
#include "wifiequd.h"
#include "wfq/config.h"
#include "wfq/mocksignal.h"
#include "wfq/wifisignal.h"
#include "wfqapi/http.h"
#include "wfqapi/json.h"

/*
 * internals
 */

static volatile sig_atomic_t g_running = 1;
static volatile sig_atomic_t g_reload = 0;
static wfq_config_context s_config_ctx;

static long clamp_refresh_wait(unsigned long configured, long fallback)
{
    if (configured == 0UL)
        return fallback;
    if (configured > (unsigned long)LONG_MAX)
        return LONG_MAX;
    return (long)configured;
}

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
    fprintf(out, "%-25s %d\n", WFQ_PARAM_HTTP_PORT, s_config_ctx.opt.port);
    fprintf(out, "%-25s %s\n", WFQ_PARAM_LOG_JSON, (s_config_ctx.opt.json_log ? "true" : "false"));
    fprintf(out, "%-25s %s\n", WFQ_PARAM_MOCK, (s_config_ctx.opt.mock ? "true" : "false"));
    fprintf(out, "%-25s %lu\n", WFQ_PARAM_REFRESH_MILLIS, s_config_ctx.opt.refresh_ms);
    dmot_ui_ostream_repeat_pattern_endl(out, "=", 80);
    fputs("\n", out);
}

static void set_up_signal_source(void)
{
    DMOT_LOGD("Setting up the signal source...");
    if (s_config_ctx.opt.mock)
    {
        wfq_mock_signal_options o = {0};
        o.n_channels = s_config_ctx.n_chan_defined;
        o.refresh_wait_ms = clamp_refresh_wait(s_config_ctx.opt.refresh_ms, WFQ_DEFAULT_SETTING_REFRESH_WAIT_MS);
        if (o.refresh_wait_ms <= 0)
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
    wfq_sample sample = (s_config_ctx.opt.mock ? wfq_sine_wave_generator_read() : wfq_wifi_signal_read());

    if (sample.readings[0].chan_id <= 0)
    {
        DMOT_LOGD("Skipping publish: no channel readings available.");
        return;
    }

    if (sample.timestamp_ms > prev_timestamp)
    {
        wfqapi_server *srv = wfqapi_get_server_context();
        wfqapi_next_sample_write(srv, &sample);
        prev_timestamp = sample.timestamp_ms;
    }
}

static void load_config(void)
{
    if (!wfq_config_read(&s_config_ctx))
    {
        DMOT_LOGE("Failed to read the configuration file at: %s", WFQ_CONFIG_PATH);
        exit(EXIT_FAILURE);
    }
    DMOT_LOGD("Loaded the configuration.");
    wfq_config_store_context(&s_config_ctx);
    DMOT_LOGD("Cached the configuration.");
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

int main(void)
{
    int exit_status = EXIT_SUCCESS;
    dmot_log_set_level(DMOT_LOG_DEBUG);
    dmot_log_set_file(stdout);
    load_config();
    kernel_set_signal_handler();
    daemon_announce(stdout);
    set_up_signal_source();

    unsigned short port = s_config_ctx.opt.port;
    if (!wfqapi_create_server_context())
    {
        DMOT_LOGE("Could not create the server context.");
        goto cleanup;
    }
    wfqapi_server *srv = wfqapi_get_server_context();
    if (!wfqapi_http_server_start(srv, port))
    {
        DMOT_LOGE("FAILED to start the API server on HTTP port %u", port);
        exit_status = EXIT_FAILURE;
        goto cleanup;
    }
    DMOT_LOGD("API server listening on HTTP port %u.", port);

    while (g_running)
    {
        if (g_reload)
        {
            reload_signal_source();
            continue;
        }
        publish_reading();
        dmot_time_sleep_ms(s_config_ctx.opt.refresh_ms);
    }

cleanup:
    wfqapi_destroy_server_context();
    tear_down_signal_source();
    DMOT_LOGD("Daemon stopped.");
    return exit_status;
}
