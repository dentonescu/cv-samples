#ifndef WFQ_WIFI_SIGNAL_H
#define WFQ_WIFI_SIGNAL_H

#include "wfq/wifiequ.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define WFQ_LIVE_SIGNAL_REFRESH_WAIT_MS 1000

    // Retrieves the most recent sample obtained from the live data source.
    // @return                  Last sample read.
    wfq_sample wfq_wifi_signal_read(void);

    // Starts the Wi-Fi signal scanner.
    // @param ctx               Configuration context.
    void wfq_wifi_signal_scanner_start(wfq_config_context *ctx);

    // Stops the Wi-Fi signal scanner.
    void wfq_wifi_signal_scanner_stop(void);

#ifdef __cplusplus
}
#endif
#endif // WFQ_WIFI_SIGNAL_H