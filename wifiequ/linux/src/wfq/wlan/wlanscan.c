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
    int rc_put_1 = nla_put(ssids, 1 /* entry index */, 0, "");
    if (rc_put_1 < 0)
    {
        nlmsg_free(msg);
        nlmsg_free(ssids);
        return rc_put_1;
    }

    int rc_put_2 = nla_put_nested(msg, NL80211_ATTR_SCAN_SSIDS, ssids);
    nlmsg_free(ssids);
    if (rc_put_2 < 0)
    {
        nlmsg_free(msg);
        return rc_put_2;
    }

    rc = nl_send_auto(sock, msg);
    nlmsg_free(msg);

    if (rc < 0)
        return rc;

    return 0;
}

// Blocks execution until NL is finished or there was an error.
// @return          true if success, false if error.
static bool wfq_block_until_called(wfq_scan_context *ctx)
{
    int rc;
    if (!ctx)
    {
        DMOT_LOGD("The context is empty. Blocking unnecessary.");
        return true;
    }
    while (!ctx->done && ctx->error == 0)
    {
        rc = nl_recvmsgs_default(ctx->sock);
        if (rc < 0)
            ctx->error = rc;
    }
    if (ctx->error < 0)
    {
        DMOT_LOGE("wfq_scan_wlan: nl_recvmsgs_default failed: %s", nl_geterror(ctx->error));
        return false;
    }
    return true;
}

static void wfq_iteration_clear_trackers(wfq_scan_context *ctx)
{
    if (!ctx)
        return;
    ctx->done = false;
    ctx->error = 0;
    ctx->n_count_results = 0;
    ctx->current_scan_results_index = 0;
}

static void wfq_init_context(wfq_scan_context *ctx)
{
    if (!ctx)
    {
        DMOT_LOGE("The context supplied was empty.");
        return;
    }
    memset(ctx->current_scan_results, 0, sizeof(ctx->current_scan_results));
    ctx->current_scan_results_index = 0;
    memset(ctx->largest_scan_results, 0, sizeof(ctx->largest_scan_results));
    ctx->largest_scan_results_index = 0;
    ctx->sock = NULL;
    ctx->family_id = -1;
    ctx->ifindex = 0;
    ctx->results = ctx->current_scan_results;
    ctx->n_max_results = WFQ_WLAN_SCAN_MAX_RESULTS;
    ctx->n_count_results = ctx->current_scan_results_index;
    ctx->done = false;
    ctx->error = 0;
    ctx->overflow = false;
    ctx->msg_dealloc_required = false;
    ctx->msg = NULL;
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
    static wfq_scan_context ctx; // static because we return a member to the outside world and need it in memory
    const char *device = (ifname && ifname[0] != '\0') ? ifname : WFQ_DEFAULT_WLAN_IFACE;

    // initialize our context
    wfq_init_context(&ctx);

    // retrieve the numeric index of the wireless interface
    ctx.ifindex = if_nametoindex(device);
    if (ctx.ifindex == 0)
    {
        DMOT_LOGE("wfq_scan_wlan: unknown interface '%s': %s", device, strerror(errno));
        return ctx.current_scan_results;
    }

    // allocate and initalize a nl_sock struct
    ctx.sock = nl_socket_alloc();
    if (!ctx.sock)
    {
        DMOT_LOGE("wfq_scan_wlan: nl_socket_alloc failed");
        return ctx.current_scan_results;
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

    // set the valid response callback
    rc = nl_socket_modify_cb(ctx.sock, NL_CB_VALID, NL_CB_CUSTOM, wfq_cb_get_scan_valid, &ctx);
    if (rc != 0)
    {
        DMOT_LOGE("wfq_scan_wlan: unable to set valid callback: %s", nl_geterror(rc));
        goto cleanup;
    }

    // set the error response callback
    rc = nl_socket_modify_err_cb(ctx.sock, NL_CB_CUSTOM, wfq_cb_get_scan_error, &ctx);
    if (rc != 0)
    {
        DMOT_LOGE("wfq_scan_wlan: unable to set error callback: %s", nl_geterror(rc));
        goto cleanup;
    }

    // set the completion callback
    rc = nl_socket_modify_cb(ctx.sock, NL_CB_FINISH, NL_CB_CUSTOM, wfq_cb_get_scan_finish, &ctx);
    if (rc != 0)
    {
        DMOT_LOGE("wfq_scan_wlan: unable to set finish callback: %s", nl_geterror(rc));
        goto cleanup;
    }

    int loop_counter = 0;
    bool trigger_scan_required = true;
    long retrigger_iterations = 100;
    long wait_start_ms = dmot_time_now_ms();

    do
    {
        wfq_iteration_clear_trackers(&ctx);

        // allocate an expandable Netlink message buffer
        ctx.msg = nlmsg_alloc();
        if (!ctx.msg)
        {
            DMOT_LOGE("wfq_scan_wlan: nlmsg_alloc failed");
            goto cleanup;
        }
        ctx.msg_dealloc_required = true;

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
                      loop_counter, ctx.current_scan_results_index, ctx.largest_scan_results_index);
        }

        // write the generic-netlink header into the message buffer and request a scan and dump
        void *hdr = genlmsg_put(ctx.msg, 0, 0, ctx.family_id, 0, NLM_F_DUMP, NL80211_CMD_GET_SCAN, 0);
        if (!hdr)
        {
            DMOT_LOGE("wfq_scan_wlan: genlmsg_put failed");
            goto cleanup;
        }

        // append the right metadata to the message so that the kernel can parse it
        rc = nla_put_u32(ctx.msg, NL80211_ATTR_IFINDEX, (uint32_t)ctx.ifindex);
        if (rc != 0)
        {
            DMOT_LOGE("wfq_scan_wlan: nla_put_u32 failed: %s", nl_geterror(rc));
            goto cleanup;
        }

        // finalize the message (padding, header lengths) and send it through the socket
        rc = nl_send_auto(ctx.sock, ctx.msg);
        if (rc < 0)
        {
            DMOT_LOGE("wfq_scan_wlan: nl_send_auto failed: %s", nl_geterror(rc));
            goto cleanup;
        }

        if (!wfq_block_until_called(&ctx))
            goto cleanup;

        ctx.current_scan_results_index = ctx.n_count_results;
        ctx.current_scan_results[ctx.current_scan_results_index] = (wfq_signal){0.0, 0.0}; // sentinel

        if (ctx.current_scan_results_index > ctx.largest_scan_results_index)
        {
            for (size_t i = 0; i < ctx.current_scan_results_index; ++i)
            {
                ctx.largest_scan_results[i].freq_mhz = ctx.current_scan_results[i].freq_mhz;
                ctx.largest_scan_results[i].strength_dbm = ctx.current_scan_results[i].strength_dbm;
            }
            ctx.largest_scan_results_index = ctx.current_scan_results_index;
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

        nlmsg_free(ctx.msg);
        ctx.msg_dealloc_required = false;

    } while (
        ((dmot_time_now_ms() - wait_start_ms) < WFQ_WLAN_SCAN_TIMEOUT_MS &&
         ctx.largest_scan_results_index < WFQ_WLAN_SCAN_QUOTA_MIN / 2));

    if (ctx.error == 0)
        DMOT_LOGD("wfq_scan_wlan: captured %zu signals.", ctx.current_scan_results_index);
    else
        DMOT_LOGE("wfq_scan_wlan: scan failed: %s", nl_geterror(ctx.error));

cleanup:
    if (ctx.msg_dealloc_required)
        nlmsg_free(ctx.msg);
    wfq_cleanup_socket(&ctx.sock);
    return ctx.largest_scan_results;
}