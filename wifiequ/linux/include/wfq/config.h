#ifndef WFQ_CONFIG_H
#define WFQ_CONFIG_H

#include "wfq/wifiequ.h"

/** @file wfq/config.h
 *  @brief Configuration context and helpers for WiFiEqu.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/** Maximum channel identifier supported by the configuration parser. */
#define WFQ_CONFIG_MAX_CHAN 1024
/** Absolute path to the daemon configuration file. */
#define WFQ_CONFIG_PATH "/etc/wifiequd.conf"
/** Configuration key controlling the capture interface. */
#define WFQ_PARAM_INTERFACE "interface"
/** Configuration key toggling JSON logging. */
#define WFQ_PARAM_LOG_JSON "log.daemon.json"
/** Configuration key enabling mock-signal mode. */
#define WFQ_PARAM_MOCK "mock"
/** Configuration key selecting the HTTP port. */
#define WFQ_PARAM_HTTP_PORT "http.port"
/** Configuration key specifying the refresh cadence in milliseconds. */
#define WFQ_PARAM_REFRESH_MILLIS "refresh.millis"

    /**
     * @brief Describes a Wi-Fi channel frequency bin.
     */
    typedef struct
    {
        int chan;          /**< Channel number. */
        double lower_freq; /**< Lower bound of the channel center frequency in MHz. */
        double mid_freq;   /**< Midpoint frequency in MHz. */
        double upper_freq; /**< Upper bound of the channel center frequency in MHz. */
    } wfq_config_channel_bin;

    /**
     * @brief Aggregates WiFiEqu configuration settings and channel metadata.
     */
    typedef struct
    {
        wfq_options opt;                                             /**< Global daemon options. */
        wfq_config_channel_bin channel_bin[WFQ_CONFIG_MAX_CHAN + 1]; /**< Channel metadata indexed by channel number. */
        int n_chan_defined;                                          /**< Count of configured channels. */
        int chan_list[WFQ_CONFIG_MAX_CHAN + 1];                      /**< Zero-indexed list of defined channel numbers. */
    } wfq_config_context;

    /**
     * @brief Loads WiFiEqu configuration from disk.
     *
     * @param[in,out] ctx Configuration context to populate.
     * @retval true Configuration parsed successfully.
     * @retval false Failed to read or parse the configuration file.
     */
    bool wfq_config_read(wfq_config_context *ctx);

    /**
     * @brief Finds the index of a channel within the defined-channel list.
     *
     * @param[in] ctx Configuration context.
     * @param[in] chan Channel number to look up.
     * @return Zero-based index when present, otherwise -1.
     */
    int wfq_config_find_index_among_channels_defined(wfq_config_context *ctx, int chan);

    /**
     * @brief Converts a frequency to the closest defined Wi-Fi channel.
     *
     * @param[in] ctx Configuration context.
     * @param[in] freq Frequency in MHz.
     * @return Channel number when a mapping exists; otherwise 0.
     */
    int wfq_config_freq2chan(wfq_config_context *ctx, double freq);

    /**
     * @brief Returns the configuration context cached by wfq_config_store_context().
     *
     * @return Pointer to the cached configuration, or `NULL` if none has been stored.
     */
    wfq_config_context *wfq_config_retrieve_context(void);

    /**
     * @brief Caches the supplied configuration context for later reuse.
     *
     * The pointer is stored verbatim; the caller retains ownership of the memory.
     *
     * @param[in] ctx Configuration context to expose globally.
     */
    void wfq_config_store_context(wfq_config_context *ctx);

#ifdef __cplusplus
}
#endif

#endif // WFQ_CONFIG_H
