#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmot/log.h"
#include "dmot/string.h"
#include "wfq/config.h"
#include "wfq/wifiequ.h"
#include "wfqapi/http.h"
#include "wfqapi/json.h"
#include "wfqapi/endpoints/stats.h"

/*
 * internals
 */

static wfq_config_context *ctx_cfg;

static int retrieve_config_context(struct MHD_Connection *conn)
{
    ctx_cfg = wfq_config_retrieve_context();
    if (!ctx_cfg)
    {
        DMOT_LOGE("wfqapi_handle_get_stats: configuration context unavailable.");
        struct MHD_Response *err = wfqapi_json_response_error(
            MHD_HTTP_INTERNAL_SERVER_ERROR, "configuration unavailable");
        int err_ret = MHD_queue_response(conn, MHD_HTTP_INTERNAL_SERVER_ERROR, err);
        MHD_destroy_response(err);
        return err_ret;
    }
    return 0;
}

static bool is_authorized_for_endpoint_stats(struct MHD_Connection *conn)
{
    if (0 == strlen(ctx_cfg->opt.access_token_stats))
    {
        DMOT_LOGE("wfqapi_handle_get_stats: The API key for this endpoint has not been set. Rejecting access.");
        return false;
    }
    const char *api_key = MHD_lookup_connection_value(conn, MHD_HEADER_KIND, WFQAPI_HDR_API_KEY);
    if (!api_key || strlen(api_key) == 0)
    {
        DMOT_LOGW("wfqapi_handle_get_stats: Missing API key header %s.", WFQAPI_HDR_API_KEY);
        return false;
    }
    if (0 == strcmp(api_key, ctx_cfg->opt.access_token_stats))
    {
        return true; // the correct API key was supplied
    }
    DMOT_LOGW("wfqapi_handle_get_stats: Invalid API key presented in header %s.", WFQAPI_HDR_API_KEY);
    return false; // by default, you are not authorized
}

/*
 * externals
 */

int wfqapi_handle_get_stats(struct MHD_Connection *conn)
{
    int ret = retrieve_config_context(conn);
    if (ret != 0)
    {
        // the configuration context was not retrieved, so an error code was sent back
        return ret;
    }
    if (!is_authorized_for_endpoint_stats(conn))
    {
        DMOT_LOGW("wfqapi_handle_get_stats: unauthorized. conn=%p", (void *)conn);
        struct MHD_Response *err = wfqapi_json_response_error(
            MHD_HTTP_UNAUTHORIZED, "unauthorized");
        int err_ret = MHD_queue_response(conn, MHD_HTTP_UNAUTHORIZED, err);
        MHD_destroy_response(err);
        return err_ret;
    }
    char json[DEFAULT_JSON_BUFFER_SIZE];
    wfqapi_config2json(ctx_cfg, json, sizeof json);
    struct MHD_Response *resp = wfqapi_json_response_ok(json);
    ret = MHD_queue_response(conn, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);
    return ret;
}
