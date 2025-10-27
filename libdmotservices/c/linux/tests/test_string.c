#include <stdarg.h>
#include <setjmp.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <cmocka.h>
#include <math.h>
#include "dmot/log.h"
#include "dmot/string.h"

static void trims_string_properly(void **state)
{
    (void)state;
    char s[] = "       spaces on the left and right        ";
    char *trimmed = dmot_string_trim(s);
    assert_string_equal(trimmed, "spaces on the left and right");
}

static void writes_into_string_properly(void **state)
{
    (void)state;
    const size_t BUF_SIZE = 8;
    char buf[BUF_SIZE];
    memset(buf, '\0', BUF_SIZE);
    char *cursor = buf;
    char *end = buf + BUF_SIZE;
    const int ret1 = dmot_string_write_into(&cursor, end, "abc");
    DMOT_LOGD("buf: %s", buf);
    const int ret2 = dmot_string_write_into(&cursor, end, "def");
    DMOT_LOGD("buf: %s", buf);
    const int ret3 = dmot_string_write_into(&cursor, end, "ghi");
    DMOT_LOGD("buf: %s", buf);
    assert_true(ret1 >= 0);
    assert_true(ret2 >= 0);
    assert_true(ret3 <= 0);
}

int main(void)
{
    dmot_log_set_level(DMOT_LOG_DEBUG);
    dmot_log_set_file(stdout);
    const struct CMUnitTest tests[] =
        {
            cmocka_unit_test(trims_string_properly),
            cmocka_unit_test(writes_into_string_properly)};
    return cmocka_run_group_tests(tests, NULL, NULL);
}
