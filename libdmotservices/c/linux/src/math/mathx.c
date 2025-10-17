#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dmot/datatypes.h"
#include "dmot/mathx.h"

/*
 * internals
 */

static bool prng_seeded = false;

static void auto_seed_prng()
{
    if (prng_seeded)
        return;
    srand((unsigned)time(NULL));
    prng_seeded = true;
}

/*
 * externally exposed
 */

double dmot_mathx_rand_double(double a, double b)
{
    if (a > b)
        return NAN;
    auto_seed_prng();
    const double amplitude = b - a;
    const double rnd_unit = (double)rand() / RAND_MAX; // [0, 1]
    return a + amplitude * rnd_unit;                   // [a, b]
}