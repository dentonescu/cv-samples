#ifndef WFQ_CONFIG_H
#define WFQ_CONFIG_H

#include "wfq/wifiequ.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define WFQ_CONFIG_MAX_CHAN 1024
#define WFQ_CONFIG_PATH "/etc/wifiequd.conf"
#define WFQ_PARAM_MOCK "mock"
#define WFQ_PARAM_HTTP_PORT "http.port"

    // Describes a channel bin.
    // @param chan          Channel number.
    // @param lower_freq    Lower frequency in MHz.
    // @param mid_freq      Mid frequency in MHz.
    // @param upper_freq    Upper frequency in MHz.
    typedef struct
    {
        int chan;
        double lower_freq;
        double mid_freq;
        double upper_freq;
    } wfq_config_channel_bin;

    // Configuration context.
    // @param opt                   WiFiEqu options.
    // @param channel_bin           Channel bin describes a Wi-Fi channel. channel_bin[1] is Wi-Fi channel 1.
    // @param n_chan_defined        Number of defined channels in the configuration.
    // @param chan_list             Zero-indexed list of defined channels.
    typedef struct
    {
        wfq_options opt;
        wfq_config_channel_bin channel_bin[WFQ_CONFIG_MAX_CHAN + 1];
        int n_chan_defined;
        int chan_list[WFQ_CONFIG_MAX_CHAN + 1]; // zero-indexed
    } wfq_config_context;

    // Retrieves configuration options from the configuration file.
    // @param ctx           wfq_config_context structure.
    // @return              true if the configuration was successfully read, false otherwise.
    bool wfq_config_read(wfq_config_context *ctx);

    // Retrieves the position of the channel in the list of defined channels.
    // @param ctx           wfq_config_context structure.
    // @return              -1 if not found, otherwise position in the array of channels defined.
    int wfq_config_find_index_among_channels_defined(wfq_config_context *ctx, int chan);

    // Converts a frequency to a channel number.
    // @param freq          Frequency (e.g. 2405)
    // @return              Channel number based on the frequency (e.g. 2405). Zero if no channel found.
    int wfq_config_freq2chan(wfq_config_context *ctx, double freq);

#ifdef __cplusplus
}
#endif

#endif // WFQ_CONFIG_H
