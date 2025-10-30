#ifndef WFQAPI_JSON_H
#define WFQAPI_JSON_H
#include <microhttpd.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Creates a JSON success response with status 200.
     *
     * Caller owns the returned `MHD_Response` and must release it with
     * `MHD_destroy_response` once queued.
     *
     * @param[in] json Null-terminated JSON payload to send.
     * @return Allocated libmicrohttpd response object, or `NULL` on failure.
     */
    struct MHD_Response *wfqapi_json_response_ok(const char *json);

    /**
     * @brief Creates a JSON error response with the supplied status code.
     *
     * Caller owns the returned `MHD_Response` and must release it with
     * `MHD_destroy_response` once queued.
     *
     * @param[in] code HTTP status code to emit (for example, 404).
     * @param[in] message Short error description encoded as JSON string content.
     * @return Allocated libmicrohttpd response object, or `NULL` on failure.
     */
    struct MHD_Response *wfqapi_json_response_error(int code, const char *message);

#ifdef __cplusplus
}
#endif
#endif // WFQAPI_JSON_H
