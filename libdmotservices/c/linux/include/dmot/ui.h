#ifndef DMOT_UI_H
#define DMOT_UI_H
#include <stdio.h>
#include "dmot/datatypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

    struct dmot_ui_eq_channel
    {
        int id;          // channel id: 1-16
        double value;    // reading (dBm)
        double smoothed; // averaged out reading in smooth mode (dBm)
    };

    typedef struct
    {
        int rows;
        int cols;
    } dmot_ui_screen_size;

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
    dmot_ui_screen_size dmot_ui_get_console_size();

    // Initializes the equalizer.
    void dmot_ui_equalizer_init();

    // Forbids the equalizer from being rendered.
    void dmot_ui_equalizer_forbid_rendering();

    // Permits the equalizer to be rendered.
    void dmot_ui_equalizer_permit_rendering();

    // Renders a text mode equalizer using the raw inputs.
    // @param forever_mode  A value of true causes the method to redraw forever.
    // @param out           Output stream. e.g. stdout.
    void dmot_ui_equalizer_render(FILE *out, bool forever_mode);

    // Renders a text mode equalizer using the smoothed inputs.
    // @param forever_mode  A value of true causes the method to redraw forever.
    // @param out       Output stream. e.g. stdout.
    void dmot_ui_equalizer_render_smoothed(FILE *out, bool forever_mode);

    // Sets the input value of a particular channel.
    // @param channel   Channel id: 1-16.
    // @param value     Input value in dBm.
    // @return          true if the value was set, false otherwise.
    bool dmot_ui_equalizer_set_channel_input_value(int channel, double value);

    // Sets how many columns wide the equalizer should be.
    // @param cols      Columns: e.g. 100
    // @return          true if the value was set, false otherwise.
    bool dmot_ui_equalizer_set_width(int cols);

    // Writes a pattern to the output stream n number of times.
    // @param out       Outputstream. e.g. stdout.
    // @param p         Pattern to be repeated. eg. "foo"
    // @param n         Number of repetitions.
    void dmot_ui_repeat_pattern(FILE *out, const char *p, size_t n);

    // Writes a pattern to the output stream n number of times. Prints a new line after.
    // @param out       Outputstream. e.g. stdout.
    // @param p         Pattern to be repeated. eg. "foo"
    // @param n         Number of repetitions.
    void dmot_ui_repeat_pattern_endl(FILE *out, const char *p, size_t n);


#define DMOT_UI_EQU_CHAR_WIDTH 40
#define DMOT_UI_EQU_DBM_LOWEST -100.0
#define DMOT_UI_EQU_DBM_HIGHEST 0.0
#define DMOT_UI_EQU_DBM_AMPLITUDE (DMOT_UI_EQU_DBM_HIGHEST - DMOT_UI_EQU_DBM_LOWEST)
#define DMOT_UI_EQU_SLEEP_MS 50
#define DMOT_UI_EQU_SMOOTHING_CONSTRAINT (0.1 * DMOT_UI_EQU_DBM_AMPLITUDE)  // no input can cause a jump greater than this value
#define DMOT_UI_EQU_MAX_CHAN 16

#ifdef __cplusplus
}
#endif
#endif // DMOT_UI_H