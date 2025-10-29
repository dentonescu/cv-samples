#include <stdarg.h>
#include <setjmp.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <cmocka.h>
#include <math.h>
#include "dmot/log.h"
#include "dmot/string.h"
#include "wfq/version.h"

static void parses_current_version_correctly(void **state)
{
    (void)state;
    char buf[32];
    const char *VERSION = wfq_version_get_version();
    DMOT_LOGD("version: %s", VERSION);
    const unsigned int major = wfq_version_get_version_major();
    const unsigned int minor = wfq_version_get_version_minor();
    const unsigned int patch = wfq_version_get_version_patch();
    DMOT_LOGD("major=%u, minor=%u, patch=%u", major, minor, patch);
    snprintf(buf, sizeof buf, "%u.%u.%u", major, minor, patch);
    assert_string_equal(VERSION, buf);
}

int main(void)
{
    dmot_log_set_level(DMOT_LOG_DEBUG);
    dmot_log_set_file(stdout);
    const struct CMUnitTest tests[] =
        {
            cmocka_unit_test(parses_current_version_correctly),
        };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
