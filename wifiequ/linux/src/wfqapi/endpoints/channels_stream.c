#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dmot/log.h"
#include "dmot/time.h"
#include "wfq/config.h"
#include "wfq/wifiequ.h"
#include "wfqapi/http.h"
#include "wfqapi/json.h"
#include "wfqapi/endpoints/channels_stream.h"

/*
 * internals
 */

static void compact_json_in_place(char *json)
{
    if (!json)
        return;
    char *r = json;
    char *w = json;
    while (*r)
    {
        if (*r != '\n' && *r != '\r' && *r != '\t')
            *w++ = *r;
        r++;
    }
    *w = '\0';
}

/*
 * callbacks
 */
static void cb_sse_done(void *cls)
{
    wfqapi_context_sse *ctx = (wfqapi_context_sse *)cls;
    if (ctx)
    {
        ctx->closed = true;
        DMOT_LOGD("cb_sse_done: ctx=%p, closed SSE stream (last_sent_ts=%llu).",
                  (void *)ctx, (unsigned long long)ctx->last_sent_ts);
    }
    free(ctx); // free(NULL) is a NOP in C
}

static ssize_t cb_sse(void *cls, uint64_t pos, char *buf, size_t max)
{
    (void)pos;
    wfqapi_context_sse *ctx = (wfqapi_context_sse *)cls;
    if (!ctx || ctx->closed)
        return MHD_CONTENT_READER_END_OF_STREAM;

    const uint64_t ts_now = dmot_time_now_ms();

    if (ts_now - ctx->last_beat_ms >= WFQAPI_STREAM_HEARTBEAT_MS)
    {
        /* No new reading yet—emit an SSE comment to keep the connection alive. */
        char ping_msg[64];
        int ping_len = snprintf(ping_msg, sizeof ping_msg, ": ping at %llu\n\n",
                                (unsigned long long)ts_now);
        if (ping_len < 0)
            return MHD_CONTENT_READER_END_OF_STREAM;
        size_t n = (size_t)ping_len;
        if (n >= sizeof ping_msg)
            n = sizeof ping_msg - 1;
        if (n > max)
            n = max;
        if (n > 0)
            memcpy(buf, ping_msg, n);
        ctx->last_beat_ms = ts_now;
        return (ssize_t)n;
    }

    wfq_sample sample = wfqapi_next_sample_read(ctx->srv);
    if ((uint64_t)sample.timestamp_ms <= ctx->last_sent_ts)
    {
        // the ring buffer read is older than the last streamed event
        return 0; // 0 => ask MHD to call again later
    }
    ctx->last_sent_ts = (uint64_t)sample.timestamp_ms;

    char json[DEFAULT_JSON_BUFFER_SIZE];
    wfqapi_sample2json(&sample, json, sizeof json);
    compact_json_in_place(json); // SSE data must be on a single line
    if (max == 0)
        return 0;

    int n = snprintf(buf, max,
                     "event: reading\n"
                     "id: %llu\n"
                     "data: %s\n\n",
                     (unsigned long long)ctx->last_sent_ts, json);

    if (n < 0)
        return MHD_CONTENT_READER_END_OF_STREAM;
    if ((size_t)n >= max)
    {
        DMOT_LOGE("cb_sse: response buffer too small for SSE payload (needed=%d, max=%zu).",
                  n, max);
        return MHD_CONTENT_READER_END_OF_STREAM;
    }
    ctx->last_beat_ms = ts_now;
    return n;
}

/*
 * externals
 */

int wfqapi_handle_get_channels_stream(struct MHD_Connection *conn)
{
    wfqapi_server *srv = wfqapi_get_server_context();
    if (!srv)
    {
        const char *error_msg = "server context unavailable";
        DMOT_LOGE("wfqapi_handle_get_channels_stream: %s.", error_msg);
        struct MHD_Response *err = wfqapi_json_response_error(
            MHD_HTTP_INTERNAL_SERVER_ERROR, error_msg);
        int erc = MHD_queue_response(conn, MHD_HTTP_INTERNAL_SERVER_ERROR, err);
        MHD_destroy_response(err);
        return erc;
    }

    wfqapi_context_sse *ctx = calloc(1, sizeof *ctx); // allocate on the heap so that each SSE client has an independent context
    if (!ctx)
    {
        const char *error_msg = "allocation failed";
        DMOT_LOGE("wfqapi_handle_get_channels_stream: %s.", error_msg);
        struct MHD_Response *err = wfqapi_json_response_error(
            MHD_HTTP_INTERNAL_SERVER_ERROR, error_msg);
        if (err)
        {
            int erc = MHD_queue_response(conn, MHD_HTTP_INTERNAL_SERVER_ERROR, err);
            MHD_destroy_response(err);
            return erc;
        }
        return MHD_NO;
    }
    ctx->srv = srv;
    ctx->last_sent_ts = 0;
    ctx->last_beat_ms = dmot_time_now_ms();

    struct MHD_Response *resp =
        MHD_create_response_from_callback(MHD_SIZE_UNKNOWN /* chunked */,
                                          WFQAPI_PREFFERED_CHUNK_SIZE,
                                          &cb_sse, ctx,
                                          &cb_sse_done);
    if (!resp)
    {
        free(ctx);
        return MHD_NO;
    }

    DMOT_LOGD("wfqapi_handle_get_channels_stream: ctx=%p, conn=%p, started SSE stream",
              (void *)ctx, (void *)conn);

    MHD_add_response_header(resp, WFQAPI_HDR_CONTENT_TYPE, WFQAPI_CT_EVENTSTREAM);
    MHD_add_response_header(resp, WFQAPI_HDR_CACHE_CTRL, WFQAPI_HDR_VAL_NOCACHE);
    MHD_add_response_header(resp, WFQAPI_HDR_CONN, WFQAPI_HDR_VAL_KEEPALIVE);
    MHD_add_response_header(resp, WFQAPI_HDR_CORS, WFQAPI_HDR_VAL_CORS);

    int ret = MHD_queue_response(conn, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);
    return ret;
}
