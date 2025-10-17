#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dmot/datatypes.h"
#include "dmot/log.h"

/*
 * internals
 */


static dmot_log_level g_lvl = DMOT_LOG_WARN;
static FILE *g_fp = NULL;

/*
 * externally exposed
 */

void dmot_log(dmot_log_level lvl, const char *fmt, ...)
{
    if (lvl > g_lvl)
        return;
    FILE *fp = g_fp ? g_fp : stderr;
    fprintf(fp, "[%s] ", dmot_log_level_name(lvl));
    va_list ap;
    va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);
    fputc('\n', fp);
}

bool dmot_log_cmp_fp(FILE *fp)
{
    return (fp == g_fp);
}

const char *dmot_log_level_name(dmot_log_level level)
{
    if (level == DMOT_LOG_DEBUG)
        return "DEBUG";
    if (level == DMOT_LOG_INFO)
        return "INFO";
    if (level == DMOT_LOG_WARN)
        return "WARN";
    if (level == DMOT_LOG_ERROR)
        return "ERROR";
    return "";
}

void dmot_log_set_file(FILE *fp)
{
    g_fp = fp;
    if (!fp)
        g_fp = stdout;
}

void dmot_log_set_level(dmot_log_level lvl)
{
    g_lvl = lvl;
}