#ifndef WFQ_MOCK_SIGNAL_H
#define WFQ_MOCK_SIGNAL_H

#include "wfq/wifiequ.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define WFQ_MOCK_SIGNAL_REFRESH_WAIT_MS 50 /**< Default delay between mock samples in milliseconds. */

    /**
     * @brief Configuration parameters for the mock signal generator.
     */
    typedef struct
    {
        int n_channels;          /**< Number of channels to synthesize. */
        long refresh_wait_ms;    /**< Delay between generated samples in milliseconds. */
    } wfq_mock_signal_options;

    /**
     * @brief Thread entry point that generates mock equalizer signals.
     *
     * @param[in] arg Pointer to a `wfq_mock_signal_options` instance.
     * @return Always `NULL`.
     */
    void *wfq_task_sine_wave_generator(void *arg);

    /**
     * @brief Initializes the sine wave signal generator with defaults.
     */
    void wfq_sine_wave_generator_init(void);

    /**
     * @brief Initializes the sine wave generator using caller-supplied options.
     *
     * @param[in] options Preferred generator settings.
     */
    void wfq_sine_wave_generator_init_with_options(wfq_mock_signal_options *options);

    /**
     * @brief Returns the most recent mock sample produced by the generator.
     *
     * @return Latest sample value.
     */
    wfq_sample wfq_sine_wave_generator_read(void);

    /**
     * @brief Starts the background thread that produces mock samples.
     */
    void wfq_sine_wave_generator_start(void);

    /**
     * @brief Stops the background thread that produces mock samples.
     */
    void wfq_sine_wave_generator_stop(void);

#ifdef __cplusplus
}
#endif
#endif // WFQ_MOCK_SIGNAL_H
