#ifndef WFQ_WIFI_SIGNAL_H
#define WFQ_WIFI_SIGNAL_H

#include "wfq/wifiequ.h"
#include "wfq/config.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define WFQ_LIVE_SIGNAL_REFRESH_WAIT_MS 1000 /**< Default polling cadence for the live signal scanner (ms). */

    /**
     * @brief Retrieves the most recent sample obtained from the live data source.
     *
     * When no fresher data is available, the previously returned sample is
     * yielded again so streaming callers can emit continuous updates without
     * handling a sentinel value.
     *
     * @return Latest live Wi-Fi sample.
     */
    wfq_sample wfq_wifi_signal_read(void);

    /**
     * @brief Starts the Wi-Fi signal scanner worker thread.
     *
     * @param[in] ctx Configuration context describing the interface and scan options.
     */
    void wfq_wifi_signal_scanner_start(wfq_config_context *ctx);

    /**
     * @brief Stops the Wi-Fi signal scanner.
     */
    void wfq_wifi_signal_scanner_stop(void);

#ifdef __cplusplus
}
#endif
#endif // WFQ_WIFI_SIGNAL_H
