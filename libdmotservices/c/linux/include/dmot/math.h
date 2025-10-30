#ifndef DMOT_MATH_H
#define DMOT_MATH_H
#include <math.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Generates a random double within a closed interval.
     *
     * @param[in] a Lower bound of the interval (inclusive).
     * @param[in] b Upper bound of the interval (inclusive).
     * @return Pseudo-random value uniformly distributed in [@p a, @p b].
     */
    double dmot_math_rand_double(double a, double b);

#ifdef __cplusplus
}
#endif
#endif // DMOT_MATH_H
