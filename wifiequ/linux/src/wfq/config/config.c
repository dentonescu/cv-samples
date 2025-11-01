#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include "wifiequd.h"
#include "dmot/log.h"
#include "dmot/string.h"
#include "wfq/wifiequ.h"
#include "wfq/config.h"

static wfq_config_context *ctx_cached;

/*
 * internals
 */

// no internals yet

/*
 * externals
 */

wfq_config_context *wfq_config_retrieve_context(void)
{
    return ctx_cached;
}

void wfq_config_store_context(wfq_config_context *ctx)
{
    ctx_cached = ctx;
}

bool wfq_config_read(wfq_config_context *ctx)
{
    if (!ctx)
    {
        DMOT_LOGE("wfq_config_read: Empty context supplied.");
        return false;
    }
    const char *path = WFQ_CONFIG_PATH;
    FILE *f = fopen(path, "r");
    if (!f)
        return false;

    // defaults
    ctx->opt.access_token_stats[0] = '\0';
    ctx->opt.interface[0] = '\0';
    ctx->opt.json_log = WFQ_DEFAULT_SETTING_JSON_LOG_MODE;
    ctx->opt.mock = WFQ_DEFAULT_SETTING_MOCK_MODE;
    ctx->opt.port = WFQ_DEFAULT_SETTING_PORT;
    ctx->opt.refresh_ms = WFQ_DEFAULT_SETTING_REFRESH_WAIT_MS;

    // initialize channel bins
    ctx->n_chan_defined = 0;
    for (int chan = 0; chan <= WFQ_CONFIG_MAX_CHAN; ++chan)
    {
        // there is no channel zero but we initialize it anyway
        ctx->chan_list[chan] = 0;
        ctx->channel_bin[chan].chan = chan;
        ctx->channel_bin[chan].lower_freq = 0;
        ctx->channel_bin[chan].mid_freq = 0;
        ctx->channel_bin[chan].upper_freq = 0;
    }

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
        if (key[0] == 'c' && key[1] == 'h')
        {
            int chan = atoi(key + 2);
            if (chan < 1 || chan > WFQ_CONFIG_MAX_CHAN)
            {
                DMOT_LOGW("Ignoring channel %d: out of range 1..%d", chan, WFQ_CONFIG_MAX_CHAN);
                continue;
            }
            if (ctx->n_chan_defined >= WFQ_CONFIG_MAX_CHAN)
            {
                DMOT_LOGW("Channel list full (>%d), dropping %d", WFQ_CONFIG_MAX_CHAN, chan);
                continue;
            }
            char *minus = strchr(val, '-'); // look for minus signs in the frequency pairs (e.g. 5650-5670)
            if (minus)
            {
                *minus = '\0';
                const char *lower_freq_str = dmot_string_trim(val);
                const char *upper_freq_str = dmot_string_trim(minus + 1);
                const double lower_freq = atof(lower_freq_str);
                const double upper_freq = atof(upper_freq_str);
                if (upper_freq >= lower_freq)
                {
                    const double mid_freq = (upper_freq + lower_freq) / 2.0;
                    ctx->channel_bin[chan].chan = chan;
                    ctx->channel_bin[chan].lower_freq = lower_freq;
                    ctx->channel_bin[chan].mid_freq = mid_freq;
                    ctx->channel_bin[chan].upper_freq = upper_freq;
                    ctx->chan_list[ctx->n_chan_defined++] = chan; // add the channel to the list, then bump up the counter
                }
            }
        }
        else if (0 == strcmp(key, WFQ_PARAM_ACCESS_TOKEN_STATS))
            snprintf(ctx->opt.access_token_stats, sizeof ctx->opt.access_token_stats, "%s", val);
        else if (0 == strcmp(key, WFQ_PARAM_INTERFACE))
            snprintf(ctx->opt.interface, sizeof ctx->opt.interface, "%s", val);
        else if (0 == strcmp(key, WFQ_PARAM_LOG_JSON))
            ctx->opt.json_log = atoi(val) != 0;
        else if (0 == strcmp(key, WFQ_PARAM_MOCK))
            ctx->opt.mock = atoi(val) != 0;
        else if (0 == strcmp(key, WFQ_PARAM_HTTP_PORT))
            ctx->opt.port = atoi(val);
        else if (0 == strcmp(key, WFQ_PARAM_REFRESH_MILLIS))
        {
            char *endptr = NULL;
            errno = 0;
            unsigned long parsed = strtoul(val, &endptr, 10);
            if (errno != 0 || endptr == val || *endptr != '\0')
            {
                DMOT_LOGW("Ignoring %s=%s: not a valid integer value.", WFQ_PARAM_REFRESH_MILLIS, val);
                continue;
            }
            if (parsed == 0UL)
            {
                DMOT_LOGW("%s must be >= 1. Using default %lu ms.", WFQ_PARAM_REFRESH_MILLIS,
                          (unsigned long)WFQ_DEFAULT_SETTING_REFRESH_WAIT_MS);
                ctx->opt.refresh_ms = WFQ_DEFAULT_SETTING_REFRESH_WAIT_MS;
            }
            else
            {
                ctx->opt.refresh_ms = parsed;
            }
        }
    }
    fclose(f);
    return true;
}

int wfq_config_find_index_among_channels_defined(wfq_config_context *ctx, int chan)
{
    if (!ctx)
    {
        DMOT_LOGE("wfq_config_find_index_among_channels_defined: Empty context supplied.");
        return -1;
    }
    for (int i = 0; i < ctx->n_chan_defined; ++i)
    {
        if (ctx->chan_list[i] == chan)
            return i;
    }
    return -1; // not found
}

int wfq_config_freq2chan(wfq_config_context *ctx, double freq)
{
    if (!ctx)
    {
        DMOT_LOGE("wfq_config_freq2chan: Empty context supplied.");
        return 0;
    }
    int chan_found = 0;
    for (int chan_list_index = 0; chan_list_index < ctx->n_chan_defined; ++chan_list_index)
    {
        const int chan = ctx->chan_list[chan_list_index];
        const double lf = ctx->channel_bin[chan].lower_freq;
        const double uf = ctx->channel_bin[chan].upper_freq;
        if ((freq + __DBL_EPSILON__) >= lf && (freq - __DBL_EPSILON__) <= uf)
        {
            chan_found = chan; // Some bins overlap, but let's keep it simple and return the first hit.
            break;
        }
    }
    return chan_found;
}
