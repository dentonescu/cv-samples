#ifndef WFQAPI_ENDPOINTS_STATS_H
#define WFQAPI_ENDPOINTS_STATS_H
#include <stdbool.h>
#include <microhttpd.h>

/** @file wfqapi/endpoints/stats.h
 *  @brief REST endpoint that exposes daemon configuration and metadata.
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Handles `GET /api/v1/stats` requests.
     *
     * Serializes the cached configuration context into a JSON payload describing
     * daemon mode, interface, refresh cadence, and configured channel bins.
     *
     * @param[in] conn libmicrohttpd connection context for the active request.
     * @retval MHD_YES Response successfully queued.
     * @retval MHD_NO  Response could not be generated due to an internal error.
     */
    int wfqapi_handle_get_stats(struct MHD_Connection *conn);

#ifdef __cplusplus
}
#endif
#endif // WFQAPI_ENDPOINTS_STATS_H
