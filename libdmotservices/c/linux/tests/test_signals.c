#include <stdarg.h>
#include <setjmp.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <cmocka.h>
#include <math.h>
#include <float.h>
#include "dmot/constants.h"
#include "dmot/signals.h"

static void sine_wave_dbm_amplitude_properly_bounded(void **state)
{
    (void)state;
    for (double k = -10; k < 10.0; k += 0.1)
    {
        for (double theta = 0.0; theta < M_PI * 200; theta += 0.1)
        {
            double dbm = dmot_signal_sine_wave_dbm(k, theta);
            assert_true(dbm <= 0.0);
            assert_true(dbm >= -100.0);
        }
    }
}

static void sine_wave_matches_formula(void **state)
{
    (void)state;
    const double A = 3.0, k = 2.0, offset = -1.5;
    for (int i = 0; i <= 360; ++i)
    {
        double theta = i * (M_PI / 180.0);
        double expected = A * sin(k * theta) + offset;
        double actual = dmot_signal_sine_wave(A, k, theta, offset);
        assert_true(fabs(expected - actual) < DBL_EPSILON);
    }
}

int main(void)
{
    const struct CMUnitTest tests[] =
        {
            cmocka_unit_test(sine_wave_dbm_amplitude_properly_bounded),
            cmocka_unit_test(sine_wave_matches_formula),
        };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
