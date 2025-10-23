#include <stdarg.h>
#include <setjmp.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <cmocka.h>
#include <math.h>
#include "dmot/string.h"

static void trims_string_properly(void **state)
{
    (void)state;
    char s[] = "       spaces on the left and right        ";
    char *trimmed = dmot_string_trim(s);
    assert_string_equal(trimmed, "spaces on the left and right");
}

int main(void)
{
    const struct CMUnitTest tests[] =
        {
            cmocka_unit_test(trims_string_properly),
        };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
