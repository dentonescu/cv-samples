#include <stdio.h>
#include "dmot/time.h"

int main(void)
{
    puts("[ ex_time ]");
    printf("Timestamp: %llu ms\n", (unsigned long long)dmot_time_now_ms());
    return 0;
}
