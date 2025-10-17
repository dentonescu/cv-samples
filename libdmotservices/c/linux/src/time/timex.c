#define _POSIX_C_SOURCE 199309L  // exposes the September 1993 POSIX API
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dmot/datatypes.h"
#include "dmot/mathx.h"
#include "dmot/timex.h"

/*
 * internals
 */

// no internals yet

/*
 * externally exposed
 */

 void sleep_ms(long ms)
 {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
 }