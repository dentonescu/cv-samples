#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmot/string.h"
#include "wfq/wifiequ.h"
#include "wfqapi/json.h"

#define CONTENT_TYPE_JSON "application/json; charset=utf-8"
#define HEADER_CONTENT_TYPE "Content-Type"

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
    MHD_add_response_header(resp, HEADER_CONTENT_TYPE, CONTENT_TYPE_JSON);
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
    MHD_add_response_header(resp, HEADER_CONTENT_TYPE, CONTENT_TYPE_JSON);
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