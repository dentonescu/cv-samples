#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmot/log.h"
#include "wfqapi/http.h"
#include "wfqapi/router.h"

/*
 * internals
 */

static const char *ERR_EMPTY_SRV_CONTEXT = "A non-empty server context must be provided.";

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

    /*
    // libmicrohttpd calls you twice for POST data; keep state via con_cls if needed.
    // For simple GET endpoints, we can dispatch directly:
    return api_router_dispatch(connection, method, url, upload_data, upload_data_size);
    */
    return MHD_NO; // to-do: remove this and call the router
}

/*
 * externals
 */

void wfqapi_sample_stream_init(wfqapi_http_server *srv)
{
    if (!srv)
        return;
    srv->sample_cursor_read = 0;
    srv->sample_cursor_read_prev = 0;
    srv->sample_cursor_write = 0;
    srv->sample_cursor_write_prev = 0;
    memset(srv->sample_stream_ring_buf, 0, sizeof(srv->sample_stream_ring_buf));
}

bool wfqapi_http_server_start(wfqapi_http_server *srv, unsigned short port)
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

    return srv->daemon != NULL;
}

void wfqapi_http_server_stop(wfqapi_http_server *srv)
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
}

wfq_sample wfqapi_next_sample_read(wfqapi_http_server *srv)
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

bool wfqapi_next_sample_write(wfqapi_http_server *srv, wfq_sample *sample)
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
        DMOT_LOGE("The supplied sample having timestamp %u ms will not be streamed as it is older than the previous sample with timestamp %u ms",
                  sample->timestamp_ms, sample_prev.timestamp_ms);
        return false;
    }
    srv->sample_stream_ring_buf[srv->sample_cursor_write] = *sample; // write
    srv->sample_cursor_write_prev = srv->sample_cursor_write;
    srv->sample_cursor_write = (srv->sample_cursor_write + 1) % WFQAPI_SAMPLE_STREAM_BUFFER_SIZE; // increment write counter and wrap around at the end
    return true;
}