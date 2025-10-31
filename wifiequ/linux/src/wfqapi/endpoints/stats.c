#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmot/log.h"
#include "wfq/config.h"
#include "wfq/wifiequ.h"
#include "wfqapi/http.h"
#include "wfqapi/json.h"
#include "wfqapi/endpoints/stats.h"

/*
 * internals
 */

// nothing yet

/*
 * externals
 */

int wfqapi_handle_get_stats(struct MHD_Connection *conn)
{
    char json[DEFAULT_JSON_BUFFER_SIZE];
    wfq_config_context *ctx_cfg = wfq_config_retrieve_context();
    if (!ctx_cfg)
    {
        DMOT_LOGE("wfqapi_handle_get_stats: configuration context unavailable.");
        struct MHD_Response *err = wfqapi_json_response_error(
            MHD_HTTP_INTERNAL_SERVER_ERROR, "configuration unavailable");
        int err_ret = MHD_queue_response(conn, MHD_HTTP_INTERNAL_SERVER_ERROR, err);
        MHD_destroy_response(err);
        return err_ret;
    }
    wfqapi_config2json(ctx_cfg, json, sizeof json);
    struct MHD_Response *resp = wfqapi_json_response_ok(json);
    int ret = MHD_queue_response(conn, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);
    return ret;
}
