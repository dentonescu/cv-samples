#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "dmot/datatypes.h"
#include "dmot/ui.h"
#include "dmot/log.h"
#include "dmot/time.h"

/*
 * internals
 */

// characters for drawing
static const char *draw_horiz_thin_line = "-"; // ─
static const char *draw_block_full = "#";      // █

// ANSI escape sequences
// very helpful guide here: https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124
static const char *ansi_black = "\e[0;30m";
static const char *ansi_blue = "\e[0;34m";
static const char *ansi_cyan = "\e[0;36m";
static const char *ansi_green = "\e[0;32m";
static const char *ansi_purple = "\e[0;35m";
static const char *ansi_red = "\e[0;31m";
static const char *ansi_reset = "\e[0m";
static const char *ansi_yellow = "\e[0;33m";
static const char *ansi_white = "\e[0;37m";

static double equ_read_channel_raw(dmot_ui_eq *eq, size_t channel)
{
    if (channel < 1 || channel > DMOT_UI_EQU_MAX_CHAN)
        return NAN;
    return eq->equ_properties.channels[channel - 1].value;
}

static double equ_read_channel_smoothed(dmot_ui_eq *eq, size_t channel)
{
    if (channel < 1 || channel > DMOT_UI_EQU_MAX_CHAN)
        return NAN;
    return eq->equ_properties.channels[channel - 1].smoothed;
}

static char *equ_power_color(double value_dbm)
{
    if (value_dbm >= -29.0)
        return "blue";
    if (value_dbm >= -49.0)
        return "green";
    if (value_dbm >= -69.0)
        return "yellow";
    return "red";
}

static void equ_write_channel(dmot_ui_eq *eq, size_t channel, double value, double smoothed)
{
    eq->equ_properties.channels[channel - 1].id = channel;
    eq->equ_properties.channels[channel - 1].value = value;
    eq->equ_properties.channels[channel - 1].smoothed = smoothed;
}

static void equ_render(dmot_ui_eq *eq, bool smoothed, int redraws)
{
    int rendered_rows = 0;
    for (int i = 0; i < redraws; ++i)
    {
        if (!eq->equ_properties.permit_rendering)
            return;
        // if this is a redraw, clear the previous drawing and go back to the start position
        if (i > 0)
        {
            for (int j = 0; j < rendered_rows; ++j)
            {
                dmot_ui_ansi_move_cursor_up(1);
                dmot_ui_ansi_clear_row();
            }
            dmot_ui_ansi_move_cursor_to_start_of_row();
        }
        rendered_rows = 0;
        fprintf(eq->out, "%s\n", ansi_reset);
        ++rendered_rows;
        // fixed width channel column, so fixed width header
        dmot_ui_ostream_repeat_pattern_endl(eq->out, draw_horiz_thin_line, eq->equ_properties.col_width);
        ++rendered_rows;
        const char *signal_raw = "Raw signal";
        const char *signal_smoothed = "Smoothed signal";
        char channel_header[DMOT_UI_EQU_CH_NAME_WIDTH + 1];
        memset(channel_header, ' ', sizeof channel_header);
        channel_header[0] = 'C';
        channel_header[1] = 'H';
        channel_header[sizeof channel_header - 1] = '\0';
        fprintf(eq->out, "%s | %s (dBm)\n",
                channel_header,
                (smoothed ? signal_smoothed : signal_raw));
        ++rendered_rows;
        dmot_ui_ostream_repeat_pattern_endl(eq->out, draw_horiz_thin_line, eq->equ_properties.col_width);
        ++rendered_rows;
        // display the channels and the power bars
        for (size_t chan = 1; chan <= eq->equ_properties.channels_available; ++chan)
        {
            char channel_label[DMOT_UI_EQU_CH_NAME_WIDTH + 1];
            if (eq->equ_properties.channels[chan - 1].ch_name[0] == '\0')
            {
                if (eq->equ_properties.render_only_named_chans)
                    continue;                                               // skip this channel if we shouldn't render channels without labels
                snprintf(channel_label, sizeof channel_label, "%zu", chan); // just show the channel number
            }
            else
            {
                snprintf(channel_label, sizeof channel_label, "%s", eq->equ_properties.channels[chan - 1].ch_name);
            }
            const int label_len = strlen(channel_label);
            if (label_len < DMOT_UI_EQU_CH_NAME_WIDTH)
            {
                memset(channel_label + label_len, ' ', sizeof channel_label - label_len);
                channel_label[sizeof channel_label - 1] = '\0';
            }
            fprintf(eq->out, "%s | ", channel_label);
            const double value = smoothed ? equ_read_channel_smoothed(eq, chan) : equ_read_channel_raw(eq, chan);
            const double power_position = value / (double)(DMOT_UI_EQU_DBM_AMPLITUDE);               // [-1.0, 0.0]
            const double normalized_power_position = power_position + 1.0;                           // [0.0, 1.0]
            const int power_bars = (normalized_power_position * eq->equ_properties.col_width) * 0.8; // 80% to prevent exceeding equalizer viewing area
            fputs(dmot_ui_ansi_esc_seq_fg(equ_power_color(value)), eq->out);
            dmot_ui_ostream_repeat_pattern(eq->out, draw_block_full, power_bars);
            fputs(ansi_reset, eq->out);
            fprintf(eq->out, " %0.1f dBm", value);
            fputc('\n', eq->out);
            ++rendered_rows;
        }
        dmot_ui_ostream_repeat_pattern_endl(eq->out, draw_horiz_thin_line, eq->equ_properties.col_width);
        ++rendered_rows;
        fprintf(eq->out, "Legend:  %sweak%s  •  %smoderate%s  •  %sstrong%s  •  %svery strong%s",
                ansi_red, ansi_reset,
                ansi_yellow, ansi_reset,
                ansi_green, ansi_reset,
                ansi_blue, ansi_reset);
        fprintf(eq->out, "%s\n", ansi_reset);
        ++rendered_rows;
        dmot_time_sleep_ms((redraws <= 1) ? 0 : eq->equ_properties.refresh_wait_ms);
    }
}

/*
 * externals
 */

void dmot_ui_ansi_clear_row()
{
    fputs("\033[2K", stdout);
    fflush(stdout);
}

const char *dmot_ui_ansi_esc_seq_fg(const char *term)
{
    if (!strcasecmp(term, "black"))
        return ansi_black;
    if (!strcasecmp(term, "blue"))
        return ansi_blue;
    if (!strcasecmp(term, "cyan"))
        return ansi_cyan;
    if (!strcasecmp(term, "green"))
        return ansi_green;
    if (!strcasecmp(term, "purple"))
        return ansi_purple;
    if (!strcasecmp(term, "red"))
        return ansi_red;
    if (!strcasecmp(term, "reset"))
        return ansi_reset;
    if (!strcasecmp(term, "yellow"))
        return ansi_yellow;
    if (!strcasecmp(term, "white"))
        return ansi_white;
    return "";
}

void dmot_ui_ansi_move_cursor_up(int rows)
{
    if (rows < 1)
        return;
    printf("\033[%dA", rows);
    fflush(stdout);
}

void dmot_ui_ansi_move_cursor_to_start_of_row()
{
    printf("\r");
    fflush(stdout);
}

struct dmot_ui_screen_size dmot_ui_get_console_size()
{
    struct winsize w;
    struct dmot_ui_screen_size s = {0};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1)
    {
        s.rows = w.ws_row;
        s.cols = w.ws_col;
    }
    return s;
}

void dmot_ui_ostream_repeat_pattern(FILE *out, const char *p, size_t n)
{
    if (!out || !p)
        return;
    for (size_t i = 0; i < n; ++i)
    {
        fputs(p, out);
    }
}

void dmot_ui_ostream_repeat_pattern_endl(FILE *out, const char *p, size_t n)
{
    dmot_ui_ostream_repeat_pattern(out, p, n);
    fputc('\n', out);
}

bool dmot_ui_equalizer_hide_chans_without_labels(dmot_ui_eq *eq, bool hide)
{
    if (!eq)
        return false;
    eq->equ_properties.render_only_named_chans = hide;
    return true;
}

void dmot_ui_equalizer_init(dmot_ui_eq *eq)
{
    eq->out = stdout;
    eq->equ_properties.channels_available = DMOT_UI_EQU_DEFAULT_N_CHAN;
    for (size_t chan = 1; chan <= DMOT_UI_EQU_MAX_CHAN; ++chan)
    {
        memset(eq->equ_properties.channels[chan - 1].ch_name, '\0', sizeof eq->equ_properties.channels[chan - 1].ch_name);
        equ_write_channel(eq, chan, DMOT_UI_EQU_DBM_LOWEST, DMOT_UI_EQU_DBM_LOWEST);
    }
    eq->equ_properties.col_width = DMOT_UI_EQU_COL_WIDTH;
    eq->equ_properties.forever_mode = true;
    eq->equ_properties.render_only_named_chans = false;
    eq->equ_properties.permit_rendering = true;
    eq->equ_properties.refresh_wait_ms = DMOT_UI_EQU_REFRESH_WAIT_MS;
    eq->equ_properties.smoothing_constraint = DMOT_UI_EQU_SMOOTHING_CONSTRAINT;
    return;
}

void dmot_ui_equalizer_render(dmot_ui_eq *eq)
{
    const bool forever_mode = eq->equ_properties.forever_mode;
    int redraws = forever_mode ? __INT_MAX__ : 1;
    equ_render(eq, false, redraws);
}

void dmot_ui_equalizer_render_smoothed(dmot_ui_eq *eq)
{
    const bool forever_mode = eq->equ_properties.forever_mode;
    int redraws = forever_mode ? __INT_MAX__ : 1;
    equ_render(eq, true, redraws);
}

bool dmot_ui_equalizer_set_channel_input_value(dmot_ui_eq *eq, size_t channel, double value)
{
    if (channel < 1 || channel > DMOT_UI_EQU_MAX_CHAN)
        return false;
    else if (value < DMOT_UI_EQU_DBM_LOWEST)
        value = DMOT_UI_EQU_DBM_LOWEST;
    else if (value > DMOT_UI_EQU_DBM_HIGHEST)
        value = DMOT_UI_EQU_DBM_HIGHEST;
    const double current_smoothed_value = equ_read_channel_smoothed(eq, channel);
    const double difference = fabs(value - current_smoothed_value);
    const double equ_smoothing_constraint = eq->equ_properties.smoothing_constraint;
    double next_smoothed_value = (value + current_smoothed_value) / 2.0;
    if (difference > equ_smoothing_constraint)
    {
        next_smoothed_value = (next_smoothed_value > current_smoothed_value) ? current_smoothed_value + equ_smoothing_constraint : current_smoothed_value - equ_smoothing_constraint;
    }
    equ_write_channel(eq, channel, value, next_smoothed_value);
    return true;
}

bool dmot_ui_equalizer_set_channel_name(dmot_ui_eq *eq, size_t chan, const char *name)
{
    if (!name || chan < 1 || chan > DMOT_UI_EQU_MAX_CHAN || strlen(name) > DMOT_UI_EQU_CH_NAME_WIDTH)
        return false;
    strncpy(eq->equ_properties.channels[chan - 1].ch_name, name, sizeof(eq->equ_properties.channels[chan - 1].ch_name));
    eq->equ_properties.channels[chan - 1].ch_name[sizeof eq->equ_properties.channels[chan - 1].ch_name - 1] = '\0';
    return true;
}

bool dmot_ui_equalizer_set_channels_available(dmot_ui_eq *eq, size_t n_chan)
{
    if (n_chan < 1 || n_chan > DMOT_UI_EQU_MAX_CHAN) // set up to the buffer size
        return false;
    eq->equ_properties.channels_available = n_chan;
    return true;
}

bool dmot_ui_equalizer_set_width(dmot_ui_eq *eq, int cols)
{
    if (cols <= 0)
        return false;
    eq->equ_properties.col_width = cols;
    return true;
}

void dmot_ui_equalizer_forbid_rendering(dmot_ui_eq *eq)
{
    eq->equ_properties.permit_rendering = false;
}

void dmot_ui_equalizer_permit_rendering(dmot_ui_eq *eq)
{
    eq->equ_properties.permit_rendering = true;
}