#ifndef DMOT_SIGNALS_H
#define DMOT_SIGNALS_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Computes a bounded sine wave sample.
     *
     * Evaluates @f$ A \cdot \sin(k \cdot \theta) + \text{offset} @f$.
     *
     * @param[in] A Amplitude applied to the sine wave.
     * @param[in] k Frequency coefficient.
     * @param[in] theta Input angle in radians.
     * @param[in] offset Vertical offset applied to the waveform.
     * @return Evaluated sine value.
     */
    double dmot_signal_sine_wave(double A, double k, double theta, double offset);

    /**
     * @brief Computes a sine wave constrained to the Wi-Fi dBm range.
     *
     * Evaluates @f$ \sin_{\text{dBm}}(k \cdot \theta) @f$ yielding values in [-100, 0] dBm.
     *
     * @param[in] k Frequency coefficient.
     * @param[in] theta Input angle in radians.
     * @return Sine value expressed in dBm.
     */
    double dmot_signal_sine_wave_dbm(double k, double theta);

#ifdef __cplusplus
}
#endif

#endif // DMOT_SIGNALS_H
