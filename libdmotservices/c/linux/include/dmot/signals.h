#ifndef DMOT_SIGNALS_H
#define DMOT_SIGNALS_H

#ifdef __cplusplus
extern "C"
{
#endif

    // Generates a bounded sine wave. f = A * SINE(k * theta) + offset
    //
    // @param A       Amplitude.
    // @param k       Cefficient.
    // @param theta   Angle in radians.
    // @param offset  Vertical offset.
    //
    // @return Computed sine value.
    double dmot_signal_sine_wave(double A, double k, double theta, double offset);

    // Generates a sine wave in a valid dBm range (-100 to 0 dBm). f = SINE_dbm(k * theta)
    //
    // @param k         Coefficient.
    // @param theta     Angle in radians.
    //
    // @return Computed sine value in dBm.
    double dmot_signal_sine_wave_dbm(double k, double theta);

#ifdef __cplusplus
}
#endif

#endif // DMOT_SIGNALS_H
