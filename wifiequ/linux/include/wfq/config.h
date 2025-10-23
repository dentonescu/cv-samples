#ifndef WFQ_CONFIG_H
#define WFQ_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

#define WFQ_PARAM_MOCK              "mock"
#define WFQ_PARAM_HTTP_PORT         "http.port"

#define WFQ_CONFIG_PATH "/etc/wifiequd.conf"

    // Retrieves configuration options from the configuration file.
    // @param opt           wfq_options structure for storing the parsed configuration values.
    // @return              true if the configuration was successfully read, false otherwise.
    bool wfq_config_read(wfq_options *opt);

#ifdef __cplusplus
}
#endif

#endif // WFQ_CONFIG_H
