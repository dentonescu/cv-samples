#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmot/log.h"
#include "wfqapi/http.h"
#include "wfqapi/router.h"
#include "wfqapi/endpoints/channels.h"
#include "wfqapi/endpoints/channels_stream.h"
#include "wfqapi/endpoints/stats.h"
#include "wfqapi/json.h"

// API base path
#define API_BASE "/api/v1"
#define API_PATH(path) API_BASE path

// API operations
#define OP_CHANNELS "/channels"
#define OP_CHANNELS_STREAM "/channels/stream"
#define OP_STATS "/stats"

// API endpoints
#define ENDPOINT_CHANNELS API_PATH(OP_CHANNELS)
#define ENDPOINT_CHANNELS_STREAM API_PATH(OP_CHANNELS_STREAM)
#define ENDPOINT_STATS API_PATH(OP_STATS)

/*
 * internals
 */

static int reply(struct MHD_Connection *conn, int status_code, const char *msg)
{
    struct MHD_Response *r = wfqapi_json_response_error(status_code, msg);
    int ret = MHD_queue_response(conn, status_code, r);
    MHD_destroy_response(r);
    return ret;
}

/*
 * externals
 */

int wfqapi_router_dispatch(struct MHD_Connection *conn,
                           const char *method,
                           const char *url,
                           const char *upload_data,
                           size_t *upload_data_size)
{
    (void)upload_data;
    (void)upload_data_size;

    if (0 == strcmp(method, MHD_HTTP_METHOD_GET))
    {
        if (0 == strcmp(url, ENDPOINT_CHANNELS))
            return wfqapi_handle_get_channels(conn);
        else if (0 == strcmp(url, ENDPOINT_STATS))
            return wfqapi_handle_get_stats(conn);
        else if (0 == strcmp(url, ENDPOINT_CHANNELS_STREAM))
            return wfqapi_handle_get_channels_stream(conn);
        return reply(conn, MHD_HTTP_NOT_FOUND, WFQAPI_MSG_NOT_FOUND);
    }
    return reply(conn, MHD_HTTP_METHOD_NOT_ALLOWED, WFQAPI_MSG_METHOD_NOT_ALLOWED);
}
