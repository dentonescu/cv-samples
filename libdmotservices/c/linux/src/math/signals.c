#include "dmot/signals.h"
#include <math.h>

double dmot_signal_sine_wave(double A, double k, double theta, double offset)
{
    return A * sin(k * theta) + offset;    
}

double dmot_signal_sine_wave_dbm(double k, double theta)
{
    return dmot_signal_sine_wave(50, k, theta, -50); // 0 to -100 dBm
}