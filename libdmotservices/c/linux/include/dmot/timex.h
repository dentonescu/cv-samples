#ifndef DMOT_TIMEX_H
#define DMOT_TIMEX_H
#include <math.h>
#include <stdio.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // Sleeps for a given number of milliseconds.
    // @param ms        Time in milliseconds. e.g. 500
    void sleep_ms(long ms);

#ifdef __cplusplus
}
#endif
#endif // DMOT_TIMEX_H