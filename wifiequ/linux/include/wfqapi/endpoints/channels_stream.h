#ifndef WFQAPI_ENDPOINTS_CHANNELS_STREAM_H
#define WFQAPI_ENDPOINTS_CHANNELS_STREAM_H
#include <stdbool.h>
#include <stdint.h>
#include <microhttpd.h>

#define WFQAPI_STREAM_HEARTBEAT_MS 5000

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Per-connection state tracked for the SSE channel stream.
     *
     * Each client receives a dedicated context so heartbeats and sample IDs can
     * advance independently of other connections.
     */
    typedef struct
    {
        wfqapi_server *srv;   /**< Pointer to the HTTP server ring buffer and metadata. */
        uint64_t last_sent_ts; /**< Millisecond timestamp of the last event emitted. */
        uint64_t last_beat_ms; /**< Millisecond timestamp of the last heartbeat comment. */
        bool closed;           /**< Flag raised once libmicrohttpd signals completion. */
    } wfqapi_context_sse;

    /**
     * @brief Handles `GET /api/v1/channels/stream` requests.
     *
     * Streams Server-Sent Event (SSE) payloads for each new sample published to the
     * ring buffer, interleaving timestamped heartbeats when no fresh data is
     * available. The SSE connection remains open until the client disconnects or
     * libmicrohttpd reports completion.
     *
     * @param[in] conn libmicrohttpd connection context for the active request.
     * @retval MHD_YES Response successfully queued.
     * @retval MHD_NO  An error occurred while preparing the SSE response.
     */
    int wfqapi_handle_get_channels_stream(struct MHD_Connection *conn);

#ifdef __cplusplus
}
#endif
#endif // WFQAPI_ENDPOINTS_CHANNELS_STREAM_H
