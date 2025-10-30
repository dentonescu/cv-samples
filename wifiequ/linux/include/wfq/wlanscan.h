#ifndef WFQ_WLAN_SCAN_H
#define WFQ_WLAN_SCAN_H
#include <stdbool.h>
#include <net/if.h>
#include <linux/nl80211.h>
#include <netlink/attr.h>
#include <netlink/errno.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <netlink/socket.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define WFQ_DEFAULT_WLAN_IFACE              "wlan0"
#define WFQ_DEFAULT_WLAN_FAMILY             "nl80211"
#define WFQ_WLAN_SCAN_MAX_RESULTS           256U
#define WFQ_WLAN_SCAN_TIMEOUT_MS            30000
#define WFQ_WLAN_SCAN_WAIT_MS               100
#define WFQ_WLAN_SCAN_QUOTA_MIN             WFQ_EQU_N_CHANNELS

#ifndef NLE_FAIL
#define NLE_FAIL NLE_FAILURE
#endif

    typedef struct
    {
        double freq_mhz;      /**< Measured center frequency in MHz. */
        double strength_dbm;  /**< Observed signal strength in dBm. */
    } wfq_signal;

    /**
     * @brief Tracks state while performing a Netlink Wi-Fi scan.
     */
    typedef struct
    {
        struct nl_sock *sock;                                   /**< libnl socket used to talk to nl80211. */
        int family_id;                                          /**< Numeric nl80211 family identifier. */
        int ifindex;                                            /**< Interface index from `if_nametoindex`. */
        wfq_signal *results;                                    /**< Caller-supplied results buffer. */
        size_t n_max_results;                                   /**< Maximum number of results the buffer can hold. */
        size_t n_count_results;                                 /**< Number of results written so far. */
        bool done;                                              /**< Set when the kernel signals scan completion. */
        bool overflow;                                          /**< Set if more results arrived than the buffer can store. */
        int error;                                              /**< Netlink error code, if any. */
        struct nl_msg *msg;                                     /**< Reusable message buffer. */
        bool msg_dealloc_required;                              /**< Tracks whether `msg` must be freed by the caller. */
        wfq_signal current_scan_results[WFQ_WLAN_SCAN_MAX_RESULTS + 1]; /**< Scan results from the most recent sweep. */
        size_t current_scan_results_index;                      /**< Index of the last entry in `current_scan_results`. */
        wfq_signal largest_scan_results[WFQ_WLAN_SCAN_MAX_RESULTS + 1]; /**< Strongest results observed in any sweep. */
        size_t largest_scan_results_index;                      /**< Index of the last entry in `largest_scan_results`. */
    } wfq_scan_context;

    /**
     * @brief Performs a Wi-Fi scan and retrieves detected signals.
     *
     * @param[in] ifname Network interface name (for example, "wlan0").
     * @return Pointer to an array of scan results owned by the scan layer.
     */
    wfq_signal *wfq_scan_wlan(const char *ifname);

#ifdef __cplusplus
}
#endif
#endif // WFQ_WLAN_SCAN_H
