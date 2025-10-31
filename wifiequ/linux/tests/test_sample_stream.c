#include <stdarg.h>
#include <setjmp.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <cmocka.h>
#include <math.h>
#include "dmot/log.h"
#include "dmot/math.h"
#include "dmot/time.h"
#include "wfqapi/http.h"

#define RING_BUFFER_STATUS(tag)                                                  \
    do                                                                           \
    {                                                                            \
        DMOT_LOGD("%s: Ring buffer status: read_prev=%zu, read=%zu, "            \
                  "write_prev=%zu, write=%zu, "                                  \
                  "test_buf_ctr=%zu",                                            \
                  (tag),                                                         \
                  ctx.srv.sample_cursor_read_prev, ctx.srv.sample_cursor_read,   \
                  ctx.srv.sample_cursor_write_prev, ctx.srv.sample_cursor_write, \
                  ctx.test_buf_ctr);                                             \
    } while (0)

typedef struct
{
    long long time_tracker;
    wfq_sample test_buf[WFQAPI_SAMPLE_STREAM_BUFFER_SIZE * 10];
    size_t test_buf_ctr;
    wfqapi_server srv;
} test_context;

static test_context ctx;

static void reset_test_state(void)
{
    ctx.time_tracker = dmot_time_now_ms();
    memset(ctx.test_buf, 0, sizeof(ctx.test_buf));
    ctx.test_buf_ctr = 0;
    wfqapi_sample_stream_init(&ctx.srv);
}

static void create_a_sample(void)
{
    ctx.test_buf[ctx.test_buf_ctr].timestamp_ms = ctx.time_tracker++; // store the value of time_tracker and then bump it up
    for (size_t chan = 1; chan < 10; ++chan)
    {
        ctx.test_buf[ctx.test_buf_ctr].readings[chan - 1].chan_dbm = dmot_math_rand_double(-100.0, 0.0);
        ctx.test_buf[ctx.test_buf_ctr].readings[chan - 1].chan_id = chan;
    }
    assert_true(wfqapi_next_sample_write(&ctx.srv, &ctx.test_buf[ctx.test_buf_ctr]));
    ctx.test_buf_ctr++;
}

static void populate_samples(void)
{
    for (size_t i = 0; i < WFQAPI_SAMPLE_STREAM_BUFFER_SIZE; ++i)
    {
        create_a_sample();
    }
}

static void reads_all_correctly(void **state)
{
    (void)state;
    reset_test_state();
    populate_samples();
    RING_BUFFER_STATUS("INIT");
    for (size_t i = 0; i < WFQAPI_SAMPLE_STREAM_BUFFER_SIZE; ++i)
    {
        wfq_sample sample_curr = wfqapi_next_sample_read(&ctx.srv);
        assert_true(sample_curr.timestamp_ms > 0);
        if (sample_curr.timestamp_ms != ctx.test_buf[i].timestamp_ms)
            RING_BUFFER_STATUS("ERROR");
        assert_int_equal(sample_curr.timestamp_ms, ctx.test_buf[i].timestamp_ms);
    }
}

static void fast_producer_slow_consumer_test(void **state)
{
    (void)state;
    reset_test_state();
    wfq_sample sample_prev = {0};
    RING_BUFFER_STATUS("INIT");
    for (size_t i = 0; i < WFQAPI_SAMPLE_STREAM_BUFFER_SIZE * 2; ++i)
    {
        create_a_sample();
        create_a_sample();
        wfq_sample sample_curr = wfqapi_next_sample_read(&ctx.srv);
        if (sample_curr.timestamp_ms < sample_prev.timestamp_ms)
            RING_BUFFER_STATUS("ERROR");
        assert_true(sample_curr.timestamp_ms >= sample_prev.timestamp_ms);
        sample_prev = sample_curr;
        assert_true(sample_prev.timestamp_ms > 0);
    }
}

static void slow_producer_fast_consumer_test(void **state)
{
    (void)state;
    reset_test_state();
    wfq_sample sample_prev = {0};
    RING_BUFFER_STATUS("INIT");
    for (size_t i = 0; i < WFQAPI_SAMPLE_STREAM_BUFFER_SIZE * 2; ++i)
    {
        create_a_sample();
        wfq_sample sample_curr_1 = wfqapi_next_sample_read(&ctx.srv);
        wfq_sample sample_curr_2 = wfqapi_next_sample_read(&ctx.srv);
        assert_int_equal(sample_curr_1.timestamp_ms, sample_curr_2.timestamp_ms); // they should be the same since the producer is slow
        if (sample_curr_1.timestamp_ms < sample_prev.timestamp_ms)
            RING_BUFFER_STATUS("ERROR, read 1 of 2");
        assert_true(sample_curr_1.timestamp_ms >= sample_prev.timestamp_ms);
        if (sample_curr_2.timestamp_ms < sample_curr_1.timestamp_ms)
            RING_BUFFER_STATUS("ERROR, read 2 of 2");
        assert_true(sample_curr_2.timestamp_ms >= sample_curr_1.timestamp_ms);
        sample_prev = sample_curr_2;
        assert_true(sample_prev.timestamp_ms > 0);
    }
}

int main(void)
{
    dmot_log_set_level(DMOT_LOG_DEBUG);
    dmot_log_set_file(stdout);
    const struct CMUnitTest tests[] =
        {
            cmocka_unit_test(reads_all_correctly),
            cmocka_unit_test(fast_producer_slow_consumer_test),
            cmocka_unit_test(slow_producer_fast_consumer_test)};
    return cmocka_run_group_tests(tests, NULL, NULL);
}
