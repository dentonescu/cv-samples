#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmot/string.h"
#include "wfq/config.h"
#include "wfq/mocksignal.h"
#include "wfq/wifiequ.h"
#include "wfqapi/http.h"
#include "wfqapi/json.h"

/*
 * internals
 */

// nothing yet

/*
 * externals
 */

struct MHD_Response *wfqapi_json_response_ok(const char *json)
{
    struct MHD_Response *resp = MHD_create_response_from_buffer(
        strlen(json), (void *)json, MHD_RESPMEM_MUST_COPY);
    MHD_add_response_header(resp, WFQAPI_HDR_CONTENT_TYPE, WFQAPI_CT_JSON);
    return resp;
}

struct MHD_Response *wfqapi_json_response_error(int code, const char *message)
{
    char buf[256];
    int n = snprintf(buf, sizeof(buf),
                     "{\"error\":{\"code\":%d,\"message\":\"%s\"}}",
                     code, message ? message : "");
    if (n < 0)
        n = 0;
    struct MHD_Response *resp = MHD_create_response_from_buffer(
        (size_t)n, (void *)buf, MHD_RESPMEM_MUST_COPY);
    MHD_add_response_header(resp, WFQAPI_HDR_CONTENT_TYPE, WFQAPI_CT_JSON);
    return resp;
}

void wfqapi_sample2json(wfq_sample *sample, char *buf, size_t buf_size)
{
    if (!buf || buf_size == 0)
        return;

    buf[0] = '\0';
    if (!sample)
        return;

    char *cursor = buf;
    char *end = buf + buf_size;

    if (!dmot_string_write_into(&cursor, end, "{\n"))
        return;
    if (!dmot_string_write_into(&cursor, end, "    \"timestamp_ms\": %lld,\n", sample->timestamp_ms))
        return;
    if (!dmot_string_write_into(&cursor, end, "    \"readings\": [\n"))
        return;

    bool first = true;
    for (size_t i = 0; i < WFQ_EQU_MAX_READINGS; ++i)
    {
        const wfq_channel *channel = &sample->readings[i];
        if (channel->chan_id <= 0)
            continue;

        if (!first && !dmot_string_write_into(&cursor, end, ",\n"))
            return;

        if (!dmot_string_write_into(&cursor, end,
                                    "        {\"chan\": %d, \"dbm\": %.1f}",
                                    channel->chan_id, channel->chan_dbm))
            return;

        first = false;
    }

    if (!dmot_string_write_into(&cursor, end, "\n    ]\n}\n"))
        return;
}

void wfqapi_config2json(wfq_config_context *ctx, char *buf, size_t buf_size)
{
    if (!buf || buf_size == 0)
        return;

    buf[0] = '\0';
    if (!ctx)
        return;

    char *cursor = buf;
    char *end = buf + buf_size;

    const char *mode = ctx->opt.mock ? "mock" : "live";
    const unsigned long refresh_ms = ctx->opt.refresh_ms;

    if (!dmot_string_write_into(&cursor, end, "{\n"))
        return;
    if (!dmot_string_write_into(&cursor, end, "    \"mode\": \"%s\",\n", mode))
        return;
    if (ctx->opt.interface[0] != '\0')
    {
        if (!dmot_string_write_into(&cursor, end, "    \"interface\": \"%s\",\n", ctx->opt.interface))
            return;
    }
    else
    {
        if (!dmot_string_write_into(&cursor, end, "    \"interface\": null,\n"))
            return;
    }
    if (!dmot_string_write_into(&cursor, end, "    \"refresh_ms\": %lu,\n", refresh_ms))
        return;
    if (!dmot_string_write_into(&cursor, end, "    \"channels_defined\": %d,\n", ctx->n_chan_defined))
        return;

    if (!dmot_string_write_into(&cursor, end, "    \"channel_bins\": ["))
        return;

    int emitted_bins = 0;
    for (int i = 0; i < ctx->n_chan_defined; ++i)
    {
        int chan = ctx->chan_list[i];
        if (chan < 1 || chan > WFQ_CONFIG_MAX_CHAN)
            continue;

        const wfq_config_channel_bin *bin = &ctx->channel_bin[chan];
        if (bin->lower_freq == 0 && bin->mid_freq == 0 && bin->upper_freq == 0)
            continue;

        const char *prefix = (emitted_bins > 0) ? ",\n" : "\n";
        if (!dmot_string_write_into(&cursor, end, "%s        {\n", prefix))
            return;
        if (!dmot_string_write_into(&cursor, end, "            \"chan\": %d,\n", bin->chan))
            return;
        if (!dmot_string_write_into(&cursor, end, "            \"lower_freq_mhz\": %g,\n", bin->lower_freq))
            return;
        if (!dmot_string_write_into(&cursor, end, "            \"mid_freq_mhz\": %g,\n", bin->mid_freq))
            return;
        if (!dmot_string_write_into(&cursor, end, "            \"upper_freq_mhz\": %g\n", bin->upper_freq))
            return;
        if (!dmot_string_write_into(&cursor, end, "        }"))
            return;
        ++emitted_bins;
    }

    if (emitted_bins > 0)
    {
        if (!dmot_string_write_into(&cursor, end, "\n    ]\n"))
            return;
    }
    else
    {
        if (!dmot_string_write_into(&cursor, end, "]\n"))
            return;
    }

    if (!dmot_string_write_into(&cursor, end, "}\n"))
        return;
}
