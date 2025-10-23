#include <stdio.h>
#include "dmot/time.h"

int main(void)
{
    printf("Timestamp: %llu ms\n", (unsigned long long)dmot_time_now_ms());
    return 0;
}
