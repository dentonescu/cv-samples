#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "dmot/constants.h"
#include "dmot/math.h"
#include "dmot/time.h"
#include "dmot/signals.h"
#include "dmot/ui.h"

static const int channels = DMOT_UI_EQU_MAX_CHAN;
static const int default_cycles = 50;
static const long default_sleep_ms = 150;
static dmot_ui_eq eq;

/*
 * declarations
 */
void example_console_details();
void example_equalizer_smoothed();
void *task_random_values(void *arg);
void *task_sine_values(void *arg);

/*
 * definitions
 */

void *task_random_values(void *arg)
{
    (void)arg;
    dmot_ui_equalizer_permit_rendering(&eq);
    for (int i = 0; i < default_cycles; ++i)
    {
        for (int chan = 1; chan <= channels; ++chan)
        {
            const double input_dbm = dmot_math_rand_double(DMOT_UI_EQU_DBM_LOWEST, DMOT_UI_EQU_DBM_HIGHEST);
            dmot_ui_equalizer_set_channel_input_value(&eq, chan, input_dbm);
        }
        dmot_time_sleep_ms(default_sleep_ms);
    }
    dmot_ui_equalizer_forbid_rendering(&eq);
    return NULL;
}

void *task_sine_values(void *arg)
{
    (void)arg;
    dmot_ui_equalizer_permit_rendering(&eq);
    for (int i = 0; i < default_cycles; ++i)
    {
        for (int chan = 1; chan <= channels; ++chan)
        {
            const double input_dbm = dmot_signal_sine_wave_dbm(1.0 / chan, i);
            dmot_ui_equalizer_set_channel_input_value(&eq, chan, input_dbm);
        }
        dmot_time_sleep_ms(default_sleep_ms);
    }
    dmot_ui_equalizer_forbid_rendering(&eq);
    return NULL;
}

void example_console_details()
{
    puts("\n\n[console_details]\n");
    const struct dmot_ui_screen_size c_screen = dmot_ui_get_console_size();
    printf("Console: rows = %d, columns = %d\n\n", c_screen.rows, c_screen.cols);
}

void example_equalizer_raw()
{
    const struct dmot_ui_screen_size c_screen = dmot_ui_get_console_size();
    puts("\n\n[equalizer_raw]\n");
    dmot_ui_equalizer_init(&eq);
    dmot_ui_equalizer_set_width(&eq, (int)(c_screen.cols * 0.8));
    for (int chan = 1; chan <= DMOT_UI_EQU_MAX_CHAN; ++chan)
    {
        char ch_name[DMOT_UI_EQU_CH_NAME_WIDTH + 1];
        snprintf(ch_name, sizeof ch_name, "sinwv%02d", chan);
        dmot_ui_equalizer_set_chanel_name(&eq, chan, ch_name);
    }
    dmot_ui_equalizer_permit_rendering(&eq);
    pthread_t thread;
    pthread_create(&thread, NULL, task_sine_values, NULL);
    dmot_ui_equalizer_render(&eq);
}

void example_equalizer_smoothed()
{
    const struct dmot_ui_screen_size c_screen = dmot_ui_get_console_size();
    puts("\n\n[equalizer_smoothed]\n");
    dmot_ui_equalizer_init(&eq);
    dmot_ui_equalizer_set_width(&eq, (int)(c_screen.cols * 0.8));
    dmot_ui_equalizer_permit_rendering(&eq);
    pthread_t thread;
    pthread_create(&thread, NULL, task_random_values, NULL);
    dmot_ui_equalizer_render_smoothed(&eq);
}

int main(void)
{
    example_console_details();
    example_equalizer_raw();
    example_equalizer_smoothed();
    puts("\n");
    return 0;
}