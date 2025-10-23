#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "dmot/string.h"
#include "wfq/wifiequ.h"
#include "wfq/config.h"

/*
 * internals
 */

// no internals yet

/*
 * externals
 */

bool wfq_config_read(wfq_options *opt)
{
    const char *path = WFQ_CONFIG_PATH;
    FILE *f = fopen(path, "r");
    if (!f)
        return false;

    // defaults
    opt->port = WFQ_PORT;
    opt->mock = WFQ_MOCK_MODE;

    // configuration processing
    char line[256];
    while (fgets(line, sizeof line, f))
    {
        char *s = dmot_string_trim(line);
        if (*s == '#' || *s == '\0')
            continue;
        char *eq = strchr(s, '='); // we want lines with an assignment (e.g. x=y)
        if (!eq)
            continue;
        *eq = '\0';
        char *key = dmot_string_trim(s);      // left half in key
        char *val = dmot_string_trim(eq + 1); // right half in val
        // insert the values into opt
        if (strcmp(key, WFQ_PARAM_MOCK) == 0)
            opt->mock = atoi(val) != 0;
        else if (strcmp(key, WFQ_PARAM_HTTP_PORT) == 0)
            opt->port = atoi(val);
    }
    fclose(f);
    return true;
}