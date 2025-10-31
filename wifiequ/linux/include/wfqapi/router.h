#ifndef WFQAPI_ROUTER_H
#define WFQAPI_ROUTER_H
#include <microhttpd.h>
#include <stdbool.h>

/** @file wfqapi/router.h
 *  @brief Request router for the WiFiEqu HTTP API.
 *
 *  Declares the dispatcher responsible for mapping libmicrohttpd requests to
 *  endpoint implementations exposed under the `/api/v1` namespace.
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Dispatches an HTTP request to the matching WiFiEqu API handler.
     *
     * This function inspects the request method and URL, then routes it to the
     * appropriate endpoint implementation. Unsupported combinations result in
     * an HTTP error response.
     *
     * @param[in] conn libmicrohttpd connection context for the active request.
     * @param[in] method HTTP verb (for example, `"GET"` or `"POST"`).
     * @param[in] url Requested path beginning with `/api/...`.
     * @param[in] upload_data Pointer to request body data; may be `NULL` for GET.
     * @param[in,out] upload_data_size Size of @p upload_data; set to `0` once consumed.
     * @retval MHD_YES The request was handled and a response queued.
     * @retval MHD_NO  The router could not handle the request.
     */
    int wfqapi_router_dispatch(struct MHD_Connection *conn,
                               const char *method,
                               const char *url,
                               const char *upload_data,
                               size_t *upload_data_size);

#ifdef __cplusplus
}
#endif
#endif // WFQAPI_ROUTER_H
