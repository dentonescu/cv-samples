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
        double freq_mhz;
        double strength_dbm;
    } wfq_signal;

    // wfq_scan_context keeps track of wlanscan's state.
    // @param sock                          libnl socket handle for Netlink traffic
    // @param family_id                     resolved numeric ID for the nl80211 generic-netlink family
    // @param ifindex                       numeric interface index returned by if_nametoindex
    // @param results                       results array
    // @param n_max_results                 maximum number of results possible
    // @param n_count_results               number of results retrieved so far
    // @param done                          set when the kernel reports the dump finished or aborted
    // @param overflow                      set when the number of results exceeds the size of the results buffer
    // @param error                         failure code in the event of an error
    // @param msg                           message buffer
    // @param msg_dealloc_required          tracks whether the message buffer needs to be freed
    // @param current_scan_results          current scan results
    // @param current_scan_results_index    marks the last result in the current scan results array
    typedef struct
    {
        struct nl_sock *sock;
        int family_id;
        int ifindex;
        wfq_signal *results;
        size_t n_max_results;
        size_t n_count_results;
        bool done;
        bool overflow;
        int error;
        struct nl_msg *msg;
        bool msg_dealloc_required;
        wfq_signal current_scan_results[WFQ_WLAN_SCAN_MAX_RESULTS + 1];
        size_t current_scan_results_index;
        wfq_signal largest_scan_results[WFQ_WLAN_SCAN_MAX_RESULTS + 1];
        size_t largest_scan_results_index;
    } wfq_scan_context;

    // Scans a wireless interface and retrieves information about the detected signals.
    // @param ifname                Interface name. e.g., wlan0
    // @return                      Signal structure wfq_signal.
    wfq_signal *wfq_scan_wlan(const char *ifname);

#ifdef __cplusplus
}
#endif
#endif // WFQ_WLAN_SCAN_H