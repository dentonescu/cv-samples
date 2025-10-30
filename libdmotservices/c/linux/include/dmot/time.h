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

    /**
     * @brief Sleeps the current thread for the requested number of milliseconds.
     *
     * @param[in] ms Duration in milliseconds.
     */
    void dmot_time_sleep_ms(long ms);

    /**
     * @brief Returns the current wall-clock time in milliseconds.
     *
     * @return Milliseconds elapsed since the Unix epoch.
     */
    uint64_t dmot_time_now_ms(void);

#ifdef __cplusplus
}
#endif
#endif // DMOT_TIME_H
