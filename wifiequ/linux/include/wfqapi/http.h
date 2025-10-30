#ifndef WFQAPI_HTTP_H
#define WFQAPI_HTTP_H
#include <microhttpd.h>
#include <stdbool.h>
#include "wfq/wifiequ.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define WFQAPI_SAMPLE_STREAM_BUFFER_SIZE 64

  /**
   * @brief Holds runtime state for the WiFiEqu HTTP server.
   */
  typedef struct
  {
    struct MHD_Daemon *daemon;                                               /**< Handle to the libmicrohttpd daemon instance. */
    unsigned short port;                                                     /**< TCP port the HTTP server currently listens on. */
    wfq_sample sample_stream_ring_buf[WFQAPI_SAMPLE_STREAM_BUFFER_SIZE + 1]; /**< Circular buffer containing recent samples (extra slot distinguishes full vs. empty). */
    size_t sample_cursor_write;                                              /**< Write pointer for the ring buffer. */
    size_t sample_cursor_write_prev;                                         /**< Index of the last slot written, used for monotonic timestamp validation. */
    size_t sample_cursor_read;                                               /**< Read pointer for the ring buffer. */
    size_t sample_cursor_read_prev;                                          /**< Index of the last slot returned to consumers. */
  } wfqapi_http_server;

  /**
   * @brief Starts the HTTP server on the requested port.
   *
   * @param[in,out] srv Pointer to the server context to initialize.
   * @param[in] port TCP port to bind the HTTP server to.
   * @retval true Server started successfully.
   * @retval false Failed to create the daemon.
   */
  bool wfqapi_http_server_start(wfqapi_http_server *srv, unsigned short port);

  /**
   * @brief Stops a running HTTP server and releases its resources.
   *
   * @param[in,out] srv Pointer to the server context to shut down.
   */
  void wfqapi_http_server_stop(wfqapi_http_server *srv);

  /**
   * @brief Retrieves the next sample from the streaming ring buffer.
   *
   * When no newer sample is available, the most recently returned sample is
   * supplied again so callers can keep streaming without special-case handling.
   *
   * @param[in,out] srv HTTP server context containing ring-buffer metadata.
   * @return The next logical sample in non-decreasing timestamp order.
   */
  wfq_sample wfqapi_next_sample_read(wfqapi_http_server *srv);

  /**
   * @brief Writes a newly captured sample into the streaming ring buffer.
   *
   * @param[in,out] srv HTTP server context containing ring-buffer metadata.
   * @param[in] sample Sample to enqueue; timestamps must be monotonic.
   * @retval true Sample accepted and written.
   * @retval false Sample rejected because it is older than the last entry.
   */
  bool wfqapi_next_sample_write(wfqapi_http_server *srv, wfq_sample *sample);

  /**
   * @brief Clears the streaming ring buffer and resets cursor positions.
   *
   * Safe to call multiple times; typically invoked before (re)starting the server.
   *
   * @param[in,out] srv HTTP server context to reset.
   */
  void wfqapi_sample_stream_init(wfqapi_http_server *srv);

#ifdef __cplusplus
}
#endif
#endif // WFQAPI_HTTP_H
