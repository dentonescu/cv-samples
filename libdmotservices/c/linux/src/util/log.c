#include "dmot/log.h"
#include <stdarg.h>
#include <stdlib.h>

static dmot_log_level g_lvl = DMOT_LOG_WARN;
static FILE *g_fp = NULL;

void dmot_log_set_level(dmot_log_level lvl)
{
    g_lvl = lvl;
}

void dmot_log_set_file(FILE *fp)
{
    if (NULL == fp)
    {
        fprintf(stderr, "ERROR: dmot_log_set_file() received a NULL pointer.\n");
        abort();
    }
    g_fp = fp;
}

void dmot_log(dmot_log_level lvl, const char *fmt, ...)
{
    if (lvl > g_lvl)
        return;
    FILE *fp = g_fp ? g_fp : stderr;
    fprintf(fp, "[%s] ", lvl_str[lvl]);
    va_list ap;
    va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);
    fputc('\n', fp);
}