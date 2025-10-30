#ifndef WFQAPI_ENDPOINTS_CHANNELS_H
#define WFQAPI_ENDPOINTS_CHANNELS_H
#include <stdbool.h>
#include <microhttpd.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Handles `GET /api/v1/channels` requests.
     *
     * Generates a JSON payload containing the most recent per-channel aggregates
     * and queues it on the provided libmicrohttpd connection.
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
