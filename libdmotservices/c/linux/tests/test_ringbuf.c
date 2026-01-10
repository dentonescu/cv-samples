#include <stdarg.h>
#include <setjmp.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <cmocka.h>
#include "dmot/ringbuf.h"

#define RINGBUF_LEN 16

static void ringbuf_fills_up_correctly(void **state)
{
    (void)state;
    ringbuf rb;
    assert_true(dmot_ringbuf_init(&rb, RINGBUF_LEN, sizeof(int)));
    for (int i = 0; i < RINGBUF_LEN; ++i)
    {
        assert_int_equal(dmot_ringbuf_write(&rb, &i), RINGBUF_STATUS_OK);
    }
    assert_int_equal(dmot_ringbuf_check_status_write(&rb), RINGBUF_STATUS_FULL);
    for (int i = 0; i < RINGBUF_LEN; ++i)
    {
        int val;
        assert_int_equal(dmot_ringbuf_read(&rb, &val), RINGBUF_STATUS_OK);
        assert_int_equal(val, i);
    }
    assert_int_equal(dmot_ringbuf_check_status_read(&rb), RINGBUF_STATUS_NO_DATA);
    assert_true(dmot_ringbuf_destroy(&rb));
}

static void ringbuf_wraps_correctly(void **state)
{
    (void)state;
    ringbuf rb;
    const int N_TEST_SLOTS = 4;
    assert_true(dmot_ringbuf_init(&rb, RINGBUF_LEN, sizeof(int)));
    for (int i = 0; i < RINGBUF_LEN; ++i)
    {
        assert_int_equal(dmot_ringbuf_write_int(&rb, i), RINGBUF_STATUS_OK);
    }
    for (int i = 0; i < N_TEST_SLOTS; ++i)
    {
        int val;
        assert_int_equal(dmot_ringbuf_read_int(&rb, &val), RINGBUF_STATUS_OK);
        assert_int_equal(val, i);
    }
    for (int i = RINGBUF_LEN; i < RINGBUF_LEN + N_TEST_SLOTS; ++i)
    {
        assert_int_equal(dmot_ringbuf_write_int(&rb, i), RINGBUF_STATUS_OK);
    }
    for (int i = N_TEST_SLOTS; i < RINGBUF_LEN + N_TEST_SLOTS; ++i)
    {
        int val;
        assert_int_equal(dmot_ringbuf_read_int(&rb, &val), RINGBUF_STATUS_OK);
        assert_int_equal(val, i);
    }
    assert_int_equal(dmot_ringbuf_check_status_read(&rb), RINGBUF_STATUS_NO_DATA);
    assert_true(dmot_ringbuf_destroy(&rb));
}

static void ringbuf_type_mismatch_errors(void **state)
{
    (void)state;
    ringbuf rb;
    assert_true(dmot_ringbuf_init(&rb, RINGBUF_LEN, sizeof(int)));
    double value = 0.0;
    assert_int_equal(dmot_ringbuf_read_double(&rb, &value), RINGBUF_STATUS_ERROR);
    assert_int_equal(dmot_ringbuf_write_double(&rb, 1.0), RINGBUF_STATUS_ERROR);
    assert_true(dmot_ringbuf_destroy(&rb));
}

int main(void)
{
    const struct CMUnitTest tests[] =
        {
            cmocka_unit_test(ringbuf_fills_up_correctly),
            cmocka_unit_test(ringbuf_wraps_correctly),
            cmocka_unit_test(ringbuf_type_mismatch_errors),
        };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
