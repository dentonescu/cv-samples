#define _POSIX_C_SOURCE 199309L // exposes the September 1993 POSIX API
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "dmot/datatypes.h"
#include "dmot/math.h"
#include "dmot/time.h"

/*
 * internals
 */

// no internals yet

/*
 * externals
 */

void dmot_time_sleep_ms(long ms)
{
   struct timespec ts;
   ts.tv_sec = ms / 1000;
   ts.tv_nsec = (ms % 1000) * 1000000L;
   nanosleep(&ts, NULL);
}

uint64_t dmot_time_now_ms(void)
{
   struct timespec ts;
   clock_gettime(CLOCK_REALTIME, &ts);
   return (uint64_t)ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
}