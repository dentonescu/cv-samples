#ifndef DMOT_MATH_H
#define DMOT_MATH_H
#include <math.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // Generates a random double in the range [a, b]
    // @param a         Lower bound. e.g. -6.28
    // @param b         Upper bound. e.g. 3.14
    // @return          Random double in the range [a, b].
    double dmot_math_rand_double(double a, double b);

#ifdef __cplusplus
}
#endif
#endif // DMOT_MATH_H