#include <stdarg.h>
#include <setjmp.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <cmocka.h>
#include <math.h>
#include "dmot/constants.h"
#include "dmot/mathx.h"

static void double_random_properly_bounded(void **state)
{
    (void)state;
    for (double a = -10.0; a <= 10.0; ++a)
    {
        for (double b = a; b <= 10.0; ++b)
        {
            double d = dmot_mathx_rand_double(a, b);
            assert_true(d + __DBL_EPSILON__ >= a);
            assert_true(d <= b + __DBL_EPSILON__);
        }
    }
}

int main(void)
{
    const struct CMUnitTest tests[] =
        {
            cmocka_unit_test(double_random_properly_bounded),
        };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
