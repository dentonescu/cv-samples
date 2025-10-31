#ifndef WFQ_WIFIEQU_H
#define WFQ_WIFIEQU_H
#include <stdbool.h>

/** @file wfq/wifiequ.h
 *  @brief Core data structures shared across the WiFiEqu daemon and API.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/** Default number of Wi-Fi channels tracked by the equalizer. */
#define WFQ_EQU_N_CHANNELS 13
/** Maximum number of channel readings sampled per snapshot. */
#define WFQ_EQU_MAX_READINGS 8192
/** Maximum length of an interface name (characters). */
#define WFQ_WIDTH_INTERFACE 64

    /**
     * @brief Configuration options for the WiFiEqu daemon.
     */
    typedef struct
    {
        char interface[WFQ_WIDTH_INTERFACE + 1]; /**< Wireless network interface name (for example, `wlan0`). */
        bool json_log;                          /**< Emit JSON payloads to the daemon log when enabled. */
        bool mock;                              /**< When true, operate in mock mode instead of live capture. */
        int port;                               /**< TCP port for the HTTP API. */
        unsigned long refresh_ms;               /**< Target delay between published samples in milliseconds. */
    } wfq_options;

    /**
     * @brief Represents a single Wi-Fi channel reading.
     */
    typedef struct
    {
        int chan_id;     /**< Wi-Fi channel identifier (1-based). */
        double chan_dbm; /**< Signal strength for the channel in dBm. */
    } wfq_channel;

    /**
     * @brief Collection of channel readings captured at a given timestamp.
     */
    typedef struct
    {
        long long timestamp_ms;                     /**< Millisecond timestamp of the sample capture. */
        wfq_channel readings[WFQ_EQU_MAX_READINGS]; /**< Per-channel readings gathered for the sample. */
    } wfq_sample;

#ifdef __cplusplus
}
#endif
#endif // WFQ_WIFIEQU_H
