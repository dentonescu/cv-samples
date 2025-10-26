#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "dmot/datatypes.h"

/*
 * internals
 */

// nothing yet

/*
 * externals
 */

int size_t_to_int(size_t value)
{
    if (value > (size_t)INT_MAX)
    {
        return INT_MAX; // too big for int
    }
    return (int)value;
}
