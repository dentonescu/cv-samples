#include <stdarg.h>
#include <setjmp.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <cmocka.h>
#include "dmot/log.h"

static void test_logger_levels_respected(void **state)
{
    (void)state;
    char buf[8192];

    FILE *fpLog = fopen("test_log.log", "w+");
    assert_non_null(fpLog);

    dmot_log_level testLvl = DMOT_LOG_WARN;
    dmot_log_set_level(DMOT_LOG_WARN);
    dmot_log_set_file(fpLog);

    DMOT_LOGD("debug line for log level %d should NOT appear", testLvl);
    DMOT_LOGI("info line for log level %d should NOT appear", testLvl);
    DMOT_LOGW("warn line for log level %d should appear", testLvl);
    DMOT_LOGE("error line for log level %d should appear", testLvl);

    fflush(fpLog);
    fseek(fpLog, 0, SEEK_SET);

    size_t n = fread(buf, 1, sizeof(buf) - 1, fpLog);
    buf[n] = '\0';
    fclose(fpLog);

    assert_non_null(strstr(buf, lvl_str[DMOT_LOG_ERROR]));
    assert_non_null(strstr(buf, lvl_str[DMOT_LOG_WARN]));
    assert_null(strstr(buf, lvl_str[DMOT_LOG_INFO]));
    assert_null(strstr(buf, lvl_str[DMOT_LOG_DEBUG]));
}

int main(void)
{
    const struct CMUnitTest tests[] =
        {
            cmocka_unit_test(test_logger_levels_respected),
        };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
