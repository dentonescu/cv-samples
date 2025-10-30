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

    /**
     * @brief Holds runtime values for a single equalizer channel.
     */
    struct dmot_ui_eq_channel
    {
        int id;                                      /**< Channel identifier (1-based). */
        char ch_name[DMOT_UI_EQU_CH_NAME_WIDTH + 1]; /**< Optional short label rendered instead of the id. */
        double value;                                /**< Current signal strength in dBm. */
        double smoothed;                             /**< Smoothed value incorporating prior readings. */
    };

    /**
     * @brief Console dimensions expressed as row/column counts.
     */
    struct dmot_ui_screen_size
    {
        int rows;  /**< Visible height of the console. */
        int cols;  /**< Visible width of the console. */
    };

    /**
     * @brief Configuration and state used while rendering the equalizer.
     */
    struct dmot_ui_equ_properties
    {
        size_t channels_available;                           /**< Number of channels the UI should draw. */
        size_t col_width;                                    /**< Width allocated per channel column. */
        struct dmot_ui_eq_channel channels[DMOT_UI_EQU_MAX_CHAN + 1]; /**< Channel state storage. */
        bool forever_mode;                                   /**< When true, keep redrawing until stopped. */
        bool permit_rendering;                               /**< Toggle enabling/disabling redraws. */
        long refresh_wait_ms;                                /**< Delay between refresh cycles. */
        bool render_only_named_chans;                        /**< Hide channels without labels when true. */
        double smoothing_constraint;                         /**< Max single-step change when smoothing values. */
    };

    /**
     * @brief Container for an equalizer instance tied to a specific output stream.
     */
    typedef struct
    {
        FILE *out;                                    /**< Destination stream for rendered output. */
        struct dmot_ui_equ_properties equ_properties; /**< Equalizer state and configuration. */
    } dmot_ui_eq;

    /*
     * ANSI graphics functions
     */

    /**
     * @brief Clears the current console row using ANSI escape sequences.
     */
    void dmot_ui_ansi_clear_row();

    /**
     * @brief Returns an ANSI foreground color escape sequence.
     *
     * @param[in] term Color name (`"red"`, `"green"`, `"reset"`, etc.).
     * @return Escape sequence literal (for example, `"\x1b[0m"`).
     */
    const char *dmot_ui_ansi_esc_seq_fg(const char *term);

    /**
     * @brief Moves the console cursor upward.
     *
     * @param[in] rows Number of rows to move up.
     */
    void dmot_ui_ansi_move_cursor_up(int rows);

    /**
     * @brief Moves the cursor to the start of the current row.
     */
    void dmot_ui_ansi_move_cursor_to_start_of_row();

    /**
     * @brief Returns the current console size in rows and columns.
     *
     * @return Detected console dimensions.
     */
    struct dmot_ui_screen_size dmot_ui_get_console_size();

    /*
     * Multi-channel equalizer functions
     */

    /**
     * @brief Initializes an equalizer object with defaults.
     *
     * @param[in,out] eq Equalizer handle to initialize.
     */
    void dmot_ui_equalizer_init(dmot_ui_eq *eq);

    /**
     * @brief Prevents the equalizer from rendering future frames.
     *
     * @param[in,out] eq Equalizer handle.
     */
    void dmot_ui_equalizer_forbid_rendering(dmot_ui_eq *eq);

    /**
     * @brief Toggles visibility of channels that lack labels.
     *
     * @param[in,out] eq Equalizer handle.
     * @param[in] hide Whether to hide unlabeled channels.
     * @retval true Operation succeeded.
     * @retval false Requested change was out of range.
     */
    bool dmot_ui_equalizer_hide_chans_without_labels(dmot_ui_eq *eq, bool hide);

    /**
     * @brief Allows the equalizer to resume rendering frames.
     *
     * @param[in,out] eq Equalizer handle.
     */
    void dmot_ui_equalizer_permit_rendering(dmot_ui_eq *eq);

    /**
     * @brief Renders a text-mode equalizer using raw channel values.
     *
     * @param[in] eq Equalizer handle.
     */
    void dmot_ui_equalizer_render(dmot_ui_eq *eq);

    /**
     * @brief Renders a text-mode equalizer using smoothed channel values.
     *
     * @param[in] eq Equalizer handle.
     */
    void dmot_ui_equalizer_render_smoothed(dmot_ui_eq *eq);

    /**
     * @brief Updates the raw input value for a channel.
     *
     * @param[in,out] eq Equalizer handle.
     * @param[in] channel Channel identifier (1-based).
     * @param[in] value Signal strength in dBm.
     * @retval true Update succeeded.
     * @retval false Channel was out of range.
     */
    bool dmot_ui_equalizer_set_channel_input_value(dmot_ui_eq *eq, size_t channel, double value);

    /**
     * @brief Assigns a human-readable label to a channel.
     *
     * @param[in,out] eq Equalizer handle.
     * @param[in] chan Channel identifier (1-based).
     * @param[in] name Null-terminated label to store.
     * @retval true Label applied successfully.
     * @retval false Channel was out of range.
     */
    bool dmot_ui_equalizer_set_channel_name(dmot_ui_eq *eq, size_t chan, const char *name);

    /**
     * @brief Sets how many channels the equalizer should render.
     *
     * @param[in,out] eq Equalizer handle.
     * @param[in] n_chan Number of channels to display.
     * @retval true Update succeeded.
     * @retval false Requested count exceeded the maximum.
     */
    bool dmot_ui_equalizer_set_channels_available(dmot_ui_eq *eq, size_t n_chan);

    /**
     * @brief Adjusts the equalizer width in console columns.
     *
     * @param[in,out] eq Equalizer handle.
     * @param[in] cols Desired width in columns.
     * @retval true Width updated successfully.
     * @retval false Width was outside of the supported range.
     */
    bool dmot_ui_equalizer_set_width(dmot_ui_eq *eq, int cols);

    /*
     * Output stream functions
     */

    /**
     * @brief Writes a pattern to an output stream repeatedly.
     *
     * @param[in,out] out Destination stream (for example, `stdout`).
     * @param[in] p Null-terminated pattern to write.
     * @param[in] n Number of repetitions.
     */
    void dmot_ui_ostream_repeat_pattern(FILE *out, const char *p, size_t n);

    /**
     * @brief Writes a repeated pattern followed by a newline.
     *
     * @param[in,out] out Destination stream.
     * @param[in] p Null-terminated pattern to write.
     * @param[in] n Number of repetitions.
     */
    void dmot_ui_ostream_repeat_pattern_endl(FILE *out, const char *p, size_t n);

#ifdef __cplusplus
}
#endif
#endif // DMOT_UI_H
