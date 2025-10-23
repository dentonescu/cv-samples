#ifndef DMOT_TIME_H
#define DMOT_TIME_H
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // Sleeps for a given number of milliseconds.
    // @param ms        Time in milliseconds. e.g. 500
    void dmot_time_sleep_ms(long ms);

    // Returns the current timestamp in milliseconds.
    // @return          Current timestamp in milliseconds.
    uint64_t dmot_time_now_ms(void);

#ifdef __cplusplus
}
#endif
#endif // DMOT_TIME_H