#ifndef WFQ_WIFIEQU_H
#define WFQ_WIFIEQU_H
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

// defaults
#define WFQ_EQU_N_CHANNELS          13
#define WFQ_EQU_MAX_READINGS        8192
#define WFQ_MOCK_MODE               false
#define WFQ_PORT                    8080
#define WFQ_REFRESH_WAIT_MS         500
#define WFQ_WIDTH_INTERFACE         64

    /**
     * @brief Configuration options for the WiFiEqu daemon.
     */
    typedef struct
    {
        bool mock;                                  /**< When true, operate in mock mode instead of live capture. */
        char interface[WFQ_WIDTH_INTERFACE + 1];    /**< Wireless network interface name (for example, `wlan0`). */
        int port;                                   /**< TCP port for the HTTP API. */
    } wfq_options;

    /**
     * @brief Represents a single Wi-Fi channel reading.
     */
    typedef struct
    {
        int chan_id;        /**< Wi-Fi channel identifier (1-based). */
        double chan_dbm;    /**< Signal strength for the channel in dBm. */
    } wfq_channel;

    /**
     * @brief Collection of channel readings captured at a given timestamp.
     */
    typedef struct
    {
        long long timestamp_ms;                         /**< Millisecond timestamp of the sample capture. */
        wfq_channel readings[WFQ_EQU_MAX_READINGS];     /**< Per-channel readings gathered for the sample. */
    } wfq_sample;

    /**
     * @brief Serializes a sample into JSON form.
     *
     * @param[in] sample Sample to encode.
     * @param[out] buf Destination buffer for UTF-8 JSON output.
     * @param[in] buf_size Capacity of @p buf in bytes.
     */
    void wfq_sample2json(wfq_sample *sample, char *buf, size_t buf_size);

#ifdef __cplusplus
}
#endif
#endif // WFQ_WIFIEQU_H
