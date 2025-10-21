#ifndef WFQ_WIFI_SIGNAL_H
#define WFQ_WIFI_SIGNAL_H

#include "wfq/wifiequ.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define WFQ_LIVE_SIGNAL_REFRESH_WAIT_MS 50

    //  Thread routine that continuously polls the Wi-Fi source for signal data.
    void *wfq_task_wifi_poll(void *arg);

    // Retrieves the most recent sample obtained from the live data source.
    // @return                  Last sample read.
    wfq_sample wfq_wifi_signal_read(void);

#ifdef __cplusplus
}
#endif
#endif // WFQ_WIFI_SIGNAL_H