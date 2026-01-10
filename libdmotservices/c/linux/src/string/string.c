#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dmot/string.h"

/*
 * internals
 */

// no internals yet

/*
 * externals
 */
char *dmot_string_trim(char *s)
{
    while (isspace((unsigned char)*s))
        ++s;
    if (*s == '\0')
        return s;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end))
        --end;
    end[1] = '\0';
    return s;
}

int dmot_string_write_into(char **cursor, char *end, const char *fmt, ...)
{
    size_t remaining = (size_t)(end - *cursor);
    if (remaining == 0)
        return 0;

    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(*cursor, remaining, fmt, args);
    va_end(args);

    if (written < 0)
        return written; // negative

    if ((size_t)written >= remaining)
    {
        (*cursor)[remaining - 1] = '\0';
        *cursor = end;
        return (remaining - written); // negative
    }

    *cursor += written;
    return written; // positive
}
