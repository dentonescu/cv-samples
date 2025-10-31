#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmot/log.h"
#include "wfqapi/router.h"
#include "wfqapi/endpoints/channels.h"
#include "wfqapi/json.h"

// endpoints
#define API_BASE "/api/v1"
#define API_PATH(path) API_BASE path
#define ENDPOINT_CHANNELS "/channels"
#define ENDPOINT_CHANNELS_FULL API_PATH(ENDPOINT_CHANNELS)

// HTTP response messages
#define MSG_NOT_FOUND "Not found"
#define MSG_METHOD_NOT_ALLOWED "Method not allowed"

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
        if (0 == strcmp(url, ENDPOINT_CHANNELS_FULL))
            return wfqapi_handle_get_channels(conn);
        return reply(conn, MHD_HTTP_NOT_FOUND, MSG_NOT_FOUND);
    }
    return reply(conn, MHD_HTTP_METHOD_NOT_ALLOWED, MSG_METHOD_NOT_ALLOWED);
}
