#ifndef WFQ_WIFIEQU_H
#define WFQ_WIFIEQU_H
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

// defaults
#define WFQ_EQU_N_CHANNELS 13
#define WFQ_MOCK_MODE false
#define WFQ_PORT 8080
#define WFQ_REFRESH_WAIT_MS 500

    typedef struct
    {
        int port;
        bool mock;
    } wfq_options;

    typedef struct
    {
        long long timestamp_ms;
        double channels_dbm[WFQ_EQU_N_CHANNELS];
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