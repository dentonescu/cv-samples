#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dmot/string.h"

/*
 *
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