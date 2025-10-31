#ifndef WFQ_WIFIEQUD_H
#define WFQ_WIFIEQUD_H
#include <stdbool.h>
#include "wfq/wifiequ.h"

/** @file wifiequd.h
 *  @brief Public defaults exposed by the WiFiEqu daemon binary.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/** Default JSON logging flag applied when no configuration is present. */
#define WFQ_DEFAULT_SETTING_JSON_LOG_MODE false
/** Default mock-mode flag applied when no configuration is present. */
#define WFQ_DEFAULT_SETTING_MOCK_MODE false
/** Default HTTP port used by the daemon. */
#define WFQ_DEFAULT_SETTING_PORT 8080
/** Default delay between equalizer refreshes (ms). */
#define WFQ_DEFAULT_SETTING_REFRESH_WAIT_MS 500

    // no declarations yet

#ifdef __cplusplus
}
#endif
#endif // WFQ_WIFIEQUD_H
