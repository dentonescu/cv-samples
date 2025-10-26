#ifndef DMOT_UI_H
#define DMOT_UI_H
#include <stdio.h>
#include "dmot/datatypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define DMOT_UI_EQU_CH_NAME_WIDTH 8
#define DMOT_UI_EQU_COL_WIDTH 40
#define DMOT_UI_EQU_DBM_LOWEST -100.0
#define DMOT_UI_EQU_DBM_HIGHEST 0.0
#define DMOT_UI_EQU_DBM_AMPLITUDE (DMOT_UI_EQU_DBM_HIGHEST - DMOT_UI_EQU_DBM_LOWEST)
#define DMOT_UI_EQU_REFRESH_WAIT_MS 50
#define DMOT_UI_EQU_SMOOTHING_CONSTRAINT (0.1 * DMOT_UI_EQU_DBM_AMPLITUDE) // no input can cause a jump greater than this value
#define DMOT_UI_EQU_DEFAULT_N_CHAN 16
#define DMOT_UI_EQU_MAX_CHAN 1024

    // Represents a single channel of the equalizer
    // @param id            Channel ID. E.g., 1
    // @param ch_name       When set labels the channel with a short name intead of a number.
    // @param value         Current signal strength on this channel (dBm).
    // @param smoothed      Average of the past signal strengths with an incremental movement towards the current value.
    struct dmot_ui_eq_channel
    {
        int id;
        char ch_name[DMOT_UI_EQU_CH_NAME_WIDTH + 1];
        double value;
        double smoothed;
    };

    // Represents the size of the console window.
    // @params rows             height in rows
    // @params cols             width in columns
    struct dmot_ui_screen_size
    {
        int rows;
        int cols;
    };

    // Equalizer properties.
    // @param channels_available        Number of channels available.
    // @param col_width                 Number of columns to be used for rendering.
    // @param channels                  Array of channels.
    // @param forever_mode              If true, continuously redraws the equalizer until a stop signal is received. Otherwise, draws only once.
    // @param permit_rendering          If true, allows the equalizer to be drawn. Otherwise, stops rendering of the equalizer.
    // @param refresh_wait_ms           Delay between each redraw cycle.
    // @param render_only_named_chans   Render only named channels.
    // @param smoothing_constraint      Maximum single movement in dBm permitted for a channel. Prevents jerky equalizer movements.
    struct dmot_ui_equ_properties
    {
        size_t channels_available;
        size_t col_width;
        struct dmot_ui_eq_channel channels[DMOT_UI_EQU_MAX_CHAN + 1];
        bool forever_mode;
        bool permit_rendering;
        long refresh_wait_ms;
        bool render_only_named_chans;
        double smoothing_constraint;
    };

    // Equalizer handle for creating and using an equalizer.
    // @param id            Equalizer ID.
    // @param forever_mode  Continuous rendering option.
    // @param out           Output stream.
    // @param refresh_ms    Wait period between redraws.
    typedef struct
    {
        FILE *out;
        struct dmot_ui_equ_properties equ_properties;
    } dmot_ui_eq;

    /*
     * ANSI graphics functions
     */

    // Clears the entire row in the console.
    void dmot_ui_ansi_clear_row();

    // Provides the ANSI forgeground escape sequence for various terms.
    // @param term      Term. E.g. black, blue, cyan, green, purple, red, reset, yellow, white.
    // @return          ANSI code of the requested term. e.g. "\x1b[0m"
    const char *dmot_ui_ansi_esc_seq_fg(const char *term);

    // Moves the cursor up the specified number of rows.
    // @param rows      Rows to move up.
    void dmot_ui_ansi_move_cursor_up(int rows);

    // Moves the cursor to the start of the row.
    void dmot_ui_ansi_move_cursor_to_start_of_row();

    // Determines the size of the console window and returns the number of rows and columns.
    // @return          dmot_ui_screen_size struct containing the rows and columns.
    struct dmot_ui_screen_size dmot_ui_get_console_size();

    /*
     * Multi-channel equalizer functions
     */

    // Initializes the equalizer.
    // param eq         Equalizer handle.
    void dmot_ui_equalizer_init(dmot_ui_eq *eq);

    // Forbids the equalizer from being rendered.
    // param eq         Equalizer handle.
    void dmot_ui_equalizer_forbid_rendering(dmot_ui_eq *eq);

    // Hides or reveals channels without labels. By default, channels without labels are visible.
    // @param eq        Equalizer handle.
    // @param hide      If true, hides channels without labels. Otherwise, all channels will be rendered.
    // @return          true if the value was set, false otherwise.
    bool dmot_ui_equalizer_hide_chans_without_labels(dmot_ui_eq *eq, bool hide);

    // Permits the equalizer to be rendered.
    // param eq         Equalizer handle.
    void dmot_ui_equalizer_permit_rendering(dmot_ui_eq *eq);

    // Renders a text mode equalizer using the raw inputs.
    // @param eq        Equalizer handle.
    void dmot_ui_equalizer_render(dmot_ui_eq *eq);

    // Renders a text mode equalizer using the smoothed inputs.
    // @param eq        Equalizer handle.
    void dmot_ui_equalizer_render_smoothed(dmot_ui_eq *eq);

    // Sets the input value of a particular channel.
    // @param eq        Equalizer handle.
    // @param channel   Channel id: 1-16.
    // @param value     Input value in dBm.
    // @return          true if the value was set, false otherwise.
    bool dmot_ui_equalizer_set_channel_input_value(dmot_ui_eq *eq, size_t channel, double value);

    // Sets a short name for a channel with a maximum length of DMOT_UI_EQU_CH_NAME_WIDTH
    // @param eq        Equalizer handle.
    // @param chan      Channel number. E.g. 1, 2, 3 ...
    // @param name      Label for the channel.
    // @return          true if the name was set, false otherwise.
    bool dmot_ui_equalizer_set_channel_name(dmot_ui_eq *eq, size_t chan, const char *name);

    // Set the number of channels available.
    // @param eq        Equalizer handle.
    // @param n_chan    Number of channels available. E.g. 16
    // @return          true if the value was set, false otherwise.
    bool dmot_ui_equalizer_set_channels_available(dmot_ui_eq *eq, size_t n_chan);

    // Sets how many columns wide the equalizer should be.
    // @param eq        Equalizer handle.
    // @param cols      Columns: e.g. 100
    // @return          true if the value was set, false otherwise.
    bool dmot_ui_equalizer_set_width(dmot_ui_eq *eq, int cols);

    /*
     * Output stream functions
     */

    // Writes a pattern to the output stream n number of times.
    // @param out       Output stream. e.g. stdout.
    // @param p         Pattern to be repeated. eg. "foo"
    // @param n         Number of repetitions.
    void dmot_ui_ostream_repeat_pattern(FILE *out, const char *p, size_t n);

    // Writes a pattern to the output stream n number of times. Prints a new line after.
    // @param out       Outputstream. e.g. stdout.
    // @param p         Pattern to be repeated. eg. "foo"
    // @param n         Number of repetitions.
    void dmot_ui_ostream_repeat_pattern_endl(FILE *out, const char *p, size_t n);

#ifdef __cplusplus
}
#endif
#endif // DMOT_UI_H