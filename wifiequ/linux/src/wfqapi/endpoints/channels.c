#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmot/log.h"
#include "wfq/config.h"
#include "wfq/wifiequ.h"
#include "wfqapi/http.h"
#include "wfqapi/json.h"
#include "wfqapi/endpoints/channels.h"

/*
 * internals
 */

// nothing yet

/*
 * externals
 */

int wfqapi_handle_get_channels(struct MHD_Connection *conn)
{
    char json[DEFAULT_JSON_BUFFER_SIZE];
    wfqapi_server *srv = wfqapi_get_server_context();
    wfq_sample sample = wfqapi_next_sample_read(srv);
    wfqapi_sample2json(&sample, json, sizeof json);
    wfq_config_context *ctx_cfg = wfq_config_retrieve_context();
    if (ctx_cfg && ctx_cfg->opt.json_log)
    {
        DMOT_LOGD("json=%s", json);
    }
    struct MHD_Response *resp = wfqapi_json_response_ok(json);
    int ret = MHD_queue_response(conn, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);
    return ret;
}
