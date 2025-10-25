#ifndef WFQ_WLAN_SCAN_H
#define WFQ_WLAN_SCAN_H
#include <stdbool.h>
#include <netlink/errno.h>

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
        double freq_mhz;
        double strength_dbm;
    } wfq_signal;

    typedef struct
    {
        struct nl_sock *sock;   // libnl socket handle for Netlink traffic
        int family_id;          // resolved numeric ID for the nl80211 generic-netlink family
        int ifindex;            // numeric interface index returned by if_nametoindex
        wfq_signal *results;    // results array
        size_t n_max_results;   // maximum number of results possible
        size_t n_count_results; // number of results retrieved so far
        bool done;              // set when the kernel reports the dump finished or aborted
        bool overflow;          // set when the number of results exceeds the size of the results buffer
        int error;              // failure code in the event of an error
    } wfq_scan_context;

    // Scans a wireless interface and retrieves information about the detected signals.
    // @param ifname                Interface name. e.g., wlan0
    // @return                      Signal structure wfq_signal.
    wfq_signal *wfq_scan_wlan(const char *ifname);

#ifdef __cplusplus
}
#endif
#endif // WFQ_WLAN_SCAN_H