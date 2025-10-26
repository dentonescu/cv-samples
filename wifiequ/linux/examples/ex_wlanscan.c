#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "dmot/log.h"
#include "dmot/time.h"
#include "dmot/ui.h"
#include "wfq/wifiequ.h"
#include "wfq/config.h"
#include "wfq/wlanscan.h"

/*
 * declarations
 */
static void display_signal_table(char *iface);
static void display_equalizer(char *iface);
static void *task_scan_and_render_wifi_signals(void *arg);

/*
 * definitions
 */

static wfq_config_context ctx;
static const int default_cycles = 30;
static const long default_sleep_ms = 1000;
static dmot_ui_eq eq;
static dmot_log_level log_level = DMOT_LOG_DEBUG;

static void *task_scan_and_render_wifi_signals(void *arg)
{
    char *iface = (char *)arg;
    dmot_ui_equalizer_permit_rendering(&eq);
    for (int i = 0; i < default_cycles; ++i)
    {
        size_t scan_counter = 0;
        wfq_signal *signals = wfq_scan_wlan(iface);
        while (signals[scan_counter].freq_mhz > 0 && scan_counter < WFQ_WLAN_SCAN_MAX_RESULTS)
        {
            const double freq_mhz = signals[scan_counter].freq_mhz;
            const double strength_dbm = signals[scan_counter].strength_dbm;
            const int chan_wifi = wfq_config_freq2chan(&ctx, freq_mhz);
            dmot_ui_equalizer_set_channel_input_value(&eq, chan_wifi, strength_dbm);
            ++scan_counter;
        }
        dmot_time_sleep_ms(default_sleep_ms);
    }
    dmot_ui_equalizer_forbid_rendering(&eq);
    return NULL;
}

static void display_signal_table(char *iface)
{
    puts("[display_signal_table]");
    wfq_signal *signals = wfq_scan_wlan(iface);
    if (!signals || signals[0].freq_mhz <= 0)
    {
        puts("FAILED to retrieve signal information.");
        exit(EXIT_FAILURE);
    }
    puts("");
    printf("%-10s %-12s %-10s\n", "Freq (MHz)", "Signal (dBm)", "Channel");
    printf("%-10s %-12s %-10s\n", "==========", "============", "=======");
    int i = 0;
    while (signals[i].freq_mhz > 0)
    {
        printf("%-10.2f %-12.2f %-10d\n",
               signals[i].freq_mhz,
               signals[i].strength_dbm,
               wfq_config_freq2chan(&ctx, signals[i].freq_mhz));
        ++i;
    }
    puts("");
}

static void display_equalizer(char *iface)
{
    puts("[display_equalizer]");
    dmot_log_set_level(DMOT_LOG_OFF); // don't show error messages
    const struct dmot_ui_screen_size c_screen = dmot_ui_get_console_size();
    dmot_ui_equalizer_init(&eq);
    dmot_ui_equalizer_hide_chans_without_labels(&eq, true);
    dmot_ui_equalizer_set_channels_available(&eq, DMOT_UI_EQU_MAX_CHAN);
    dmot_ui_equalizer_set_width(&eq, (int)(c_screen.cols * 0.8));
    for (int chan_list_index = 0; chan_list_index < ctx.n_chan_defined; ++chan_list_index)
    {
        const int chan = ctx.chan_list[chan_list_index];
        char ch_name[DMOT_UI_EQU_CH_NAME_WIDTH + 1];
        snprintf(ch_name, sizeof ch_name, "ch%03d", chan);
        dmot_ui_equalizer_set_channel_name(&eq, chan, ch_name);
    }
    dmot_ui_equalizer_permit_rendering(&eq);
    pthread_t thread;
    pthread_create(&thread, NULL, task_scan_and_render_wifi_signals, iface);
    dmot_ui_equalizer_render(&eq);
    puts("");
}

int main(void)
{
    puts("[ ex_wlanscan ]");
    char *iface = getenv("WFQ_IFACE");
    if (!iface)
    {
        iface = WFQ_DEFAULT_WLAN_IFACE;
        puts("WFQ_IFACE not set. Using the default interface.");
    }
    printf("Interface: %s\n", iface);
    dmot_log_set_file(stdout);
    dmot_log_set_level(log_level);
    wfq_config_read(&ctx);
    printf("\n\n%d channel(s) configured: ", ctx.n_chan_defined);
    for (int i = 0; i < ctx.n_chan_defined; ++i)
    {
        printf("%d ", ctx.chan_list[i]);
    }
    puts("\n");
    display_signal_table(iface);
    dmot_time_sleep_ms(5000);
    display_equalizer(iface);
    return EXIT_SUCCESS;
}