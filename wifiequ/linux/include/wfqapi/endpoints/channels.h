#ifndef WFQAPI_ENDPOINTS_CHANNELS_H
#define WFQAPI_ENDPOINTS_CHANNELS_H
#include <stdbool.h>
#include <microhttpd.h>

/** @file wfqapi/endpoints/channels.h
 *  @brief REST endpoint for retrieving the latest Wi-Fi channel readings.
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Handles `GET /api/v1/channels` requests.
     *
     * Serializes the latest sample published to the HTTP streaming buffer and
     * queues it as a JSON payload. When daemon JSON logging is enabled the body
     * is logged prior to transmission.
     *
     * @param[in] conn libmicrohttpd connection context for the active request.
     * @retval MHD_YES Response successfully queued.
     * @retval MHD_NO  An error occurred while generating the response.
     */
    int wfqapi_handle_get_channels(struct MHD_Connection *conn);

#ifdef __cplusplus
}
#endif
#endif // WFQAPI_ENDPOINTS_CHANNELS_H
