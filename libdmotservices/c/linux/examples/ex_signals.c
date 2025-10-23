#include <stdio.h>
#include <math.h>
#include "dmot/constants.h"
#include "dmot/signals.h"

int main(void)
{
    for (double t = 0; t < 2 * M_PI; t += 0.4)
    {
        double v = dmot_signal_sine_wave_dbm(0.5, t);
        printf("t=%.1f -> %.1f dBm\n", t, v);
    }
    return 0;
}
