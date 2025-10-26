
#ifndef WFQ_VERSION_H
#define WFQ_VERSION_H
#define WFQ_VERSION "0.2.7"

#ifdef __cplusplus
extern "C"
{
#endif

    // Retrieves the current version of wifique
    // @return          Current version of wifique
    static inline const char *wfq_version_get_version(void)
    {
        return WFQ_VERSION;
    }

#ifdef __cplusplus
}
#endif
#endif // WFQ_VERSION_H
