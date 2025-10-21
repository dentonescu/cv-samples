#include <stdio.h>
#include "dmot/timex.h"

int main(void)
{
    printf("Timestamp: %llu ms\n", (unsigned long long)timestamp_ms());
    return 0;
}
