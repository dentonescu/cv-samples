#include <stdio.h>
#include <stdlib.h>
#include "dmot/log.h"
#include "wfq/wlanscan.h"

int main(void)
{
    const char *iface = getenv("WFQ_IFACE");
    if (!iface)
    {
        iface = WFQ_DEFAULT_WLAN_IFACE;
        puts("WFQ_IFACE not set. Using the default interface.");
    }
    printf("Interface: %s\n", iface);
    dmot_log_set_file(stdout);
    dmot_log_set_level(DMOT_LOG_DEBUG);
    wfq_signal *signals = wfq_scan_wlan(iface);
    if (!signals || signals[0].freq_mhz <= 0)
    {
        puts("FAILED to retrieve signal information.");
        return EXIT_FAILURE;
    }
    printf("%-10s %-12s\n", "freq (MHz)", "signal (dBm)");
    printf("%-10s %-12s\n", "==========", "============");
    int i = 0;
    while (signals[i].freq_mhz > 0)
    {
        printf("%-10.2f %-12.2f\n",
               signals[i].freq_mhz,
               signals[i].strength_dbm);
        ++i;
    }
    return EXIT_SUCCESS;
}