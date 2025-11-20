#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmot/log.h"
#include "wfqapi/http.h"
#include "wfqapi/router.h"

#define ERR_EMPTY_SRV_CONTEXT "A non-empty server context must be provided."

/*
 * internals
 */

static wfqapi_server *srv;
static bool is_api_server_started = false;

static enum MHD_Result cb_req(void *cls,
                              struct MHD_Connection *connection,
                              const char *url,
                              const char *method,
                              const char *version,
                              const char *upload_data,
                              size_t *upload_data_size,
                              void **con_cls)
{
    (void)cls;
    (void)version;
    (void)con_cls;

    /*
     * NOTE (libmicrohttpd call sequence):
     *   1. *con_cls == NULL, *upload_data_size == 0 -> allocate per-request ctx, assign to *con_cls, return MHD_YES.
     *   2. *upload_data_size > 0 -> streaming chunk; consume it, set *upload_data_size = 0, return (no response yet).
     *   3. *upload_data_size == 0 -> upload complete; use the ctx to build/send the response.
     * Applies to POST/PUT/PATCH uploads.
     */

    return wfqapi_router_dispatch(connection, method, url, upload_data, upload_data_size);
}

/*
 * externals
 */

bool wfqapi_create_server_context(void)
{
    if (srv)
    {
        DMOT_LOGW("An attempt was made to create another server context when a handle already exists. Ignoring.");
        return false;
    }
    else
    {
        srv = calloc(1, sizeof(*srv)); // otherwise stack overflow; carries an ~8MB ring buffer
        if (!srv)
        {
            DMOT_LOGE("FAILED to allocate HTTP server state (out of memory).");
            return false;
        }
    }
    return true;
}

wfqapi_server *wfqapi_get_server_context(void)
{
    return srv;
}

bool wfqapi_destroy_server_context(void)
{
    if (srv)
    {
        if (is_api_server_started)
        {
            wfqapi_http_server_stop(srv);
            DMOT_LOGD("Stopped the API server on port %u.", srv->port);
        }
        free(srv);
        srv = NULL;
        is_api_server_started = false;
    }
    else
    {
        DMOT_LOGW("An attempt was made to destroy a non-existent server handle. Ignoring.");
        return false;
    }
    return true;
}

void wfqapi_sample_stream_init(wfqapi_server *srv)
{
    if (!srv)
        return;
    srv->sample_cursor_read = 0;
    srv->sample_cursor_read_prev = 0;
    srv->sample_cursor_write = 0;
    srv->sample_cursor_write_prev = 0;
    memset(srv->sample_stream_ring_buf, 0, sizeof(srv->sample_stream_ring_buf));
}

bool wfqapi_http_server_start(wfqapi_server *srv, unsigned short port)
{
    if (!srv)
    {
        DMOT_LOGE(ERR_EMPTY_SRV_CONTEXT);
        return false;
    }

    // initialize before use
    wfqapi_sample_stream_init(srv);

    // start microhttpd
    srv->port = port;
    srv->daemon = MHD_start_daemon(
        MHD_USE_SELECT_INTERNALLY | MHD_USE_INTERNAL_POLLING_THREAD,
        port,
        NULL, NULL,
        &cb_req, NULL,
        MHD_OPTION_END);

    is_api_server_started = (srv->daemon != NULL);
    return is_api_server_started;
}

void wfqapi_http_server_stop(wfqapi_server *srv)
{
    if (!srv)
    {
        DMOT_LOGE(ERR_EMPTY_SRV_CONTEXT);
        return;
    }
    if (srv->daemon)
    {
        MHD_stop_daemon(srv->daemon);
        srv->daemon = NULL;
    }
    is_api_server_started = false;
}

wfq_sample wfqapi_next_sample_read(wfqapi_server *srv)
{
    static wfq_sample sample_prev = {0};
    static wfq_sample sample_curr = {0};
    if (!srv)
    {
        DMOT_LOGE(ERR_EMPTY_SRV_CONTEXT);
        return sample_curr;
    }
    sample_prev = srv->sample_stream_ring_buf[srv->sample_cursor_read_prev];
    sample_curr = srv->sample_stream_ring_buf[srv->sample_cursor_read]; // read
    if (sample_prev.timestamp_ms > sample_curr.timestamp_ms)
        return sample_prev; // don't advance if the previous value is the latest available sample
    srv->sample_cursor_read_prev = srv->sample_cursor_read;
    srv->sample_cursor_read = (srv->sample_cursor_read + 1) % WFQAPI_SAMPLE_STREAM_BUFFER_SIZE; // increment read counter and wrap around at the end
    return sample_curr;
}

bool wfqapi_next_sample_write(wfqapi_server *srv, wfq_sample *sample)
{
    static wfq_sample sample_prev = {0};
    if (!srv)
    {
        DMOT_LOGE(ERR_EMPTY_SRV_CONTEXT);
        return false;
    }
    sample_prev = srv->sample_stream_ring_buf[srv->sample_cursor_write_prev];
    if (sample->timestamp_ms < sample_prev.timestamp_ms)
    {
        DMOT_LOGE("The supplied sample having timestamp %lld ms will not be streamed as it is older than the previous sample with timestamp %lld ms",
                  (long long)sample->timestamp_ms, (long long)sample_prev.timestamp_ms);
        return false;
    }
    srv->sample_stream_ring_buf[srv->sample_cursor_write] = *sample; // write
    srv->sample_cursor_write_prev = srv->sample_cursor_write;
    srv->sample_cursor_write = (srv->sample_cursor_write + 1) % WFQAPI_SAMPLE_STREAM_BUFFER_SIZE; // increment write counter and wrap around at the end
    return true;
}
