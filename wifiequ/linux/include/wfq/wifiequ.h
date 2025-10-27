#ifndef WFQ_WIFIEQU_H
#define WFQ_WIFIEQU_H
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

// defaults
#define WFQ_EQU_N_CHANNELS          13
#define WFQ_EQU_MAX_READINGS        8192
#define WFQ_MOCK_MODE               false
#define WFQ_PORT                    8080
#define WFQ_REFRESH_WAIT_MS         500
#define WFQ_WIDTH_INTERFACE         64

    // wifiequd configuration settings.
    // @param mock                      Mock mode on when true, live signal from Wi-Fi interface when false.
    // @param interface                 Wireless network interface: e.g. wlan0
    // @param port                      HTTP port to listen on.
    typedef struct
    {
        bool mock;
        char interface[WFQ_WIDTH_INTERFACE + 1];
        int port;
    } wfq_options;

    // Wi-Fi channel representation.
    // @param chan_id                   Wi-Fi channel ID. (e.g. 1)
    // @param chan_label                Short label for this channel.
    // @param chan_dbm                  Signal strength of this channel in dBm.
    typedef struct
    {
        int chan_id;
        double chan_dbm;
    } wfq_channel;

    // Signal strength readings by Wi-Fi channel.
    // @param timestamp_ms              Timestamp of the sample.
    // @param channels                  Channels and their signal strengths.
    typedef struct
    {
        long long timestamp_ms;
        wfq_channel readings[WFQ_EQU_MAX_READINGS];
    } wfq_sample;

    // Converts a sample structure to a JSON string
    // @param sample            Sample reading.
    // @param buf               String buffer to store the JSON in.
    // @param buf_size          Size of the JSON string buffer.
    void wfq_sample2json(wfq_sample *sample, char *buf, size_t buf_size);

#ifdef __cplusplus
}
#endif
#endif // WFQ_WIFIEQU_H