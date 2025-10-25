#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <net/if.h>
#include <linux/nl80211.h>
#include <netlink/attr.h>
#include <netlink/errno.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <netlink/socket.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "dmot/log.h"
#include "dmot/math.h"
#include "dmot/time.h"
#include "dmot/ui.h"
#include "wfq/wlanscan.h"
#include "wfq/wifiequ.h"

/*
 * internals
 */

static volatile wfq_signal largest_scan_results[WFQ_WLAN_SCAN_MAX_RESULTS + 1];
static volatile size_t largest_scan_results_index = 0;
static volatile wfq_signal scan_results[WFQ_WLAN_SCAN_MAX_RESULTS + 1];
static volatile size_t scan_results_index = 0;

static void wfq_cleanup_socket(struct nl_sock **sock)
{
    if (*sock != NULL)
    {
        nl_socket_free(*sock);
        *sock = NULL;
    }
}

static void wfq_add_signal(wfq_scan_context *ctx, double freq_mhz, double strength_dbm)
{
    if (ctx->n_count_results >= ctx->n_max_results)
    {
        if (!ctx->overflow)
        {
            DMOT_LOGW("wfq_scan_wlan: result list capped at %zu entries", ctx->n_max_results);
            ctx->overflow = true;
        }
        return;
    }
    ctx->results[ctx->n_count_results].freq_mhz = freq_mhz;
    ctx->results[ctx->n_count_results].strength_dbm = strength_dbm;
    ++ctx->n_count_results;
}

static int wfq_trigger_scan(struct nl_sock *sock, int family_id, int ifindex)
{
    struct nl_msg *msg = nlmsg_alloc();
    if (!msg)
        return -NLE_NOMEM;

    void *hdr = genlmsg_put(msg, 0, 0, family_id, 0, 0, NL80211_CMD_TRIGGER_SCAN, 0);
    if (!hdr)
    {
        nlmsg_free(msg);
        return -NLE_MSGSIZE;
    }

    int rc = nla_put_u32(msg, NL80211_ATTR_IFINDEX, (uint32_t)ifindex);
    if (rc != 0)
    {
        nlmsg_free(msg);
        return rc;
    }

    // Request all SSIDs by providing a single empty SSID entry.
    struct nl_msg *ssids = nlmsg_alloc();
    if (!ssids)
    {
        nlmsg_free(msg);
        return -NLE_NOMEM;
    }
    nla_put(ssids, 1 /* entry index */, 0, "");
    nla_put_nested(msg, NL80211_ATTR_SCAN_SSIDS, ssids);
    nlmsg_free(ssids);

    rc = nl_send_auto(sock, msg);
    nlmsg_free(msg);
    return (rc < 0) ? rc : 0;
}

/*
 * callbacks
 */

static int wfq_cb_get_scan_finish(struct nl_msg *msg, void *arg)
{
    (void)msg;
    wfq_scan_context *ctx = arg;
    ctx->done = true;
    return NL_OK;
}

static int wfq_cb_get_scan_error(struct sockaddr_nl *nla, struct nlmsgerr *err, void *arg)
{
    (void)nla;
    wfq_scan_context *ctx = arg;
    ctx->error = err ? err->error : -NLE_FAIL;
    ctx->done = true;
    return NL_STOP;
}

static int wfq_cb_get_scan_valid(struct nl_msg *msg, void *arg)
{
    wfq_scan_context *ctx = arg;
    struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
    struct nlattr *top_attrs[NL80211_ATTR_MAX + 1] = {0};

    static const struct nla_policy bss_policy[NL80211_BSS_MAX + 1] = {
        [NL80211_BSS_FREQUENCY] = {.type = NLA_U32},
        [NL80211_BSS_SIGNAL_MBM] = {.type = NLA_S32},
        [NL80211_BSS_SIGNAL_UNSPEC] = {.type = NLA_U8},
    };

    int rc = nla_parse(top_attrs, NL80211_ATTR_MAX,
                       genlmsg_attrdata(gnlh, 0),
                       genlmsg_attrlen(gnlh, 0), NULL);
    if (rc != 0)
    {
        ctx->error = rc;
        ctx->done = true;
        return NL_STOP;
    }

    if (!top_attrs[NL80211_ATTR_BSS])
        return NL_OK;

    struct nlattr *bss_attrs[NL80211_BSS_MAX + 1] = {0};
    rc = nla_parse_nested(bss_attrs, NL80211_BSS_MAX, top_attrs[NL80211_ATTR_BSS], bss_policy);

    if (rc != 0)
    {
        ctx->error = rc;
        ctx->done = true;
        return NL_STOP;
    }
    if (!bss_attrs[NL80211_BSS_FREQUENCY])
        return NL_OK;

    double freq_mhz = (double)nla_get_u32(bss_attrs[NL80211_BSS_FREQUENCY]);
    double strength_dbm = NAN;

    if (bss_attrs[NL80211_BSS_SIGNAL_MBM])
    {
        strength_dbm = (double)nla_get_s32(bss_attrs[NL80211_BSS_SIGNAL_MBM]) / 100.0;
    }
    else if (bss_attrs[NL80211_BSS_SIGNAL_UNSPEC])
    {
        const unsigned quality = nla_get_u8(bss_attrs[NL80211_BSS_SIGNAL_UNSPEC]);
        strength_dbm = -95.0 + 0.5 * (double)quality; // rough heuristic
    }

    if (!isnan(strength_dbm))
        wfq_add_signal(ctx, freq_mhz, strength_dbm);
    return NL_OK;
}

/*
 * externals
 */

wfq_signal *wfq_scan_wlan(const char *ifname)
{
    const char *device = (ifname && ifname[0] != '\0') ? ifname : WFQ_DEFAULT_WLAN_IFACE;

    // initialize our context
    memset(scan_results, 0, sizeof(scan_results));
    scan_results_index = 0;
    wfq_scan_context ctx =
        {
            .sock = NULL,
            .family_id = -1,
            .ifindex = 0,
            .results = scan_results,
            .n_max_results = WFQ_WLAN_SCAN_MAX_RESULTS,
            .n_count_results = scan_results_index,
            .done = false,
            .error = 0,
            .overflow = false,
        };

    // retrieve the numeric index of the wireless interface
    ctx.ifindex = if_nametoindex(device);
    if (ctx.ifindex == 0)
    {
        DMOT_LOGE("wfq_scan_wlan: unknown interface '%s': %s", device, strerror(errno));
        return scan_results;
    }

    // allocate and initalize a nl_sock struct
    ctx.sock = nl_socket_alloc();
    if (!ctx.sock)
    {
        DMOT_LOGE("wfq_scan_wlan: nl_socket_alloc failed");
        return scan_results;
    }

    // bind nl_sock struct to the generic Netlink subsystem in the kernel
    int rc = genl_connect(ctx.sock);
    if (rc != 0)
    {
        DMOT_LOGE("wfq_scan_wlan: genl_connect failed: %s", nl_geterror(rc));
        goto cleanup;
    }

    // ask the kernel's generic-netlink controller what numeric family ID corresponds to the family name
    ctx.family_id = genl_ctrl_resolve(ctx.sock, WFQ_DEFAULT_WLAN_FAMILY);
    if (ctx.family_id < 0)
    {
        DMOT_LOGE("wfq_scan_wlan: unable to resolve %s family: %s",
                  WFQ_DEFAULT_WLAN_FAMILY,
                  nl_geterror(ctx.family_id));
        goto cleanup;
    }

    int loop_counter = 0;
    bool trigger_scan_required = true;
    long retrigger_iterations = 100;
    long wait_start_ms = dmot_time_now_ms();
    do
    {
        if (trigger_scan_required)
        {
            trigger_scan_required = false;
            // trigger a fresh scan or else NL80211_CMD_GET_SCAN won't get the full set of entries
            rc = wfq_trigger_scan(ctx.sock, ctx.family_id, ctx.ifindex);
            if (rc != 0)
            {
                DMOT_LOGE("wfq_scan_wlan: trigger scan failed: %s", nl_geterror(rc));
                goto cleanup;
            }
            DMOT_LOGD("Fresh scan triggered. loop_counter=%d, scan_results_index=%d, largest_scan_results_index=%d",
                      loop_counter, scan_results_index, largest_scan_results_index);
        }

        // allocate an expandable Netlink message buffer
        struct nl_msg *msg = nlmsg_alloc();
        if (!msg)
        {
            DMOT_LOGE("wfq_scan_wlan: nlmsg_alloc failed");
            goto cleanup;
        }

        // write the generic-netlink header into the message buffer and request a scan and dump
        void *hdr = genlmsg_put(msg, 0, 0, ctx.family_id, 0, NLM_F_DUMP, NL80211_CMD_GET_SCAN, 0);
        if (hdr == NULL)
        {
            DMOT_LOGE("wfq_scan_wlan: genlmsg_put failed");
            nlmsg_free(msg);
            goto cleanup;
        }

        // append the right metadata to the message so that the kernel can parse it
        rc = nla_put_u32(msg, NL80211_ATTR_IFINDEX, (uint32_t)ctx.ifindex);
        if (rc != 0)
        {
            DMOT_LOGE("wfq_scan_wlan: nla_put_u32 failed: %s", nl_geterror(rc));
            nlmsg_free(msg);
            goto cleanup;
        }

        // set the valid response callback
        rc = nl_socket_modify_cb(ctx.sock, NL_CB_VALID, NL_CB_CUSTOM, wfq_cb_get_scan_valid, &ctx);
        if (rc != 0)
        {
            DMOT_LOGE("wfq_scan_wlan: unable to set valid callback: %s", nl_geterror(rc));
            nlmsg_free(msg);
            goto cleanup;
        }

        // set the error response callback
        rc = nl_socket_modify_err_cb(ctx.sock, NL_CB_CUSTOM, wfq_cb_get_scan_error, &ctx);
        if (rc != 0)
        {
            DMOT_LOGE("wfq_scan_wlan: unable to set error callback: %s", nl_geterror(rc));
            nlmsg_free(msg);
            goto cleanup;
        }

        // set the completion callback
        rc = nl_socket_modify_cb(ctx.sock, NL_CB_FINISH, NL_CB_CUSTOM, wfq_cb_get_scan_finish, &ctx);
        if (rc != 0)
        {
            DMOT_LOGE("wfq_scan_wlan: unable to set finish callback: %s", nl_geterror(rc));
            nlmsg_free(msg);
            goto cleanup;
        }

        // finalize the message (padding, header lengths) and send it through the socket
        rc = nl_send_auto(ctx.sock, msg);
        if (rc < 0)
        {
            DMOT_LOGE("wfq_scan_wlan: nl_send_auto failed: %s", nl_geterror(rc));
            nlmsg_free(msg);
            goto cleanup;
        }

        // release the message buffer
        nlmsg_free(msg);

        // block until the completion callback or error callback returns
        while (!ctx.done && ctx.error == 0)
        {
            rc = nl_recvmsgs_default(ctx.sock);
            if (rc < 0)
                ctx.error = rc;
        }

        scan_results_index = ctx.n_count_results;
        scan_results[scan_results_index] = (wfq_signal){0.0, 0.0}; // sentinel

        if (scan_results_index > largest_scan_results_index)
        {
            for (size_t i = 0; i < scan_results_index; ++i)
            {
                largest_scan_results[i].freq_mhz = scan_results[i].freq_mhz;
                largest_scan_results[i].strength_dbm = scan_results[i].strength_dbm;
            }
            largest_scan_results_index = scan_results_index;
        }

        // I had trouble with the initial scan's callback, hence sleep-based logic.
        // TODO: improve this with a callback!
        dmot_time_sleep_ms(WFQ_WLAN_SCAN_WAIT_MS);
        ++loop_counter;

        if ((loop_counter % retrigger_iterations) == 0)
        {
            retrigger_iterations <<= 1;
            trigger_scan_required = true;
        }

    } while (
        (largest_scan_results_index <= 1) || // This will run forever if there is only one signal or none
        ((dmot_time_now_ms() - wait_start_ms) < WFQ_WLAN_SCAN_TIMEOUT_MS &&
         largest_scan_results_index < WFQ_WLAN_SCAN_QUOTA_MIN / 2));

    if (ctx.error == 0)
        DMOT_LOGD("wfq_scan_wlan: captured %zu signals.", scan_results_index);
    else
        DMOT_LOGE("wfq_scan_wlan: scan failed: %s", nl_geterror(ctx.error));

cleanup:
    wfq_cleanup_socket(&ctx.sock);
    return largest_scan_results;
}