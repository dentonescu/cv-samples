#ifndef WFQ_MOCK_SIGNAL_H
#define WFQ_MOCK_SIGNAL_H

#include "wfq/wifiequ.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define WFQ_MOCK_SIGNAL_REFRESH_WAIT_MS 50

    typedef struct
    {
        int n_channels;
        long refresh_wait_ms;
    } wfq_mock_signal_options;

    // A thread which generates fake input signals for the equalizer using sine waves.
    void *wfq_task_sine_wave_generator(void *arg);

    // Initializes the sine wave signal generator.
    void wfq_sine_wave_generator_init(void);

    // Initializes the sine wave signal generator with user options.
    // @param options           Preferences for the generator.
    void wfq_sine_wave_generator_init_with_options(wfq_mock_signal_options *options);

    // Reads the last sample produced by the sine wave signal generator.
    // @return                  Last sample read.
    wfq_sample wfq_sine_wave_generator_read(void);

    // Starts the threaded sine wave signal generator.
    void wfq_sine_wave_generator_start(void);

    // Stops the threaded sine wave signal generator.
    void wfq_sine_wave_generator_stop(void);

#ifdef __cplusplus
}
#endif
#endif // WFQ_MOCK_SIGNAL_H