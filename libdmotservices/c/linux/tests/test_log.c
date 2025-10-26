#include <stdarg.h>
#include <setjmp.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <cmocka.h>
#include "dmot/datatypes.h"
#include "dmot/log.h"

static void null_fp_yields_stdout(void **state)
{
    (void)state;
    dmot_log_set_level(DMOT_LOG_INFO);
    dmot_log_set_file(NULL);
    assert_true(dmot_log_cmp_fp(stdout));
    DMOT_LOGI("stdout was used instead of the NULL file pointer.");
}

static void test_logger_levels_respected(void **state)
{
    (void)state;
    char buf[8192];

    FILE *fp_log = fopen("test_log.log", "w+");
    assert_non_null(fp_log);

    dmot_log_level test_level = DMOT_LOG_WARN;
    dmot_log_set_level(DMOT_LOG_WARN);
    dmot_log_set_file(fp_log);

    DMOT_LOGD("debug line for log level %d should NOT appear", test_level);
    DMOT_LOGI("info line for log level %d should NOT appear", test_level);
    DMOT_LOGW("warn line for log level %d should appear", test_level);
    DMOT_LOGE("error line for log level %d should appear", test_level);

    fflush(fp_log);
    fseek(fp_log, 0, SEEK_SET);

    size_t n = fread(buf, 1, sizeof(buf) - 1, fp_log);
    buf[n] = '\0';
    fclose(fp_log);

    assert_non_null(strstr(buf, dmot_log_level_name(DMOT_LOG_ERROR)));
    assert_non_null(strstr(buf, dmot_log_level_name(DMOT_LOG_WARN)));
    assert_null(strstr(buf, dmot_log_level_name(DMOT_LOG_INFO)));
    assert_null(strstr(buf, dmot_log_level_name(DMOT_LOG_DEBUG)));
}

int main(void)
{
    const struct CMUnitTest tests[] =
        {
            cmocka_unit_test(null_fp_yields_stdout),
            cmocka_unit_test(test_logger_levels_respected),
        };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
