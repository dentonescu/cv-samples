
#ifndef DMOT_VERSION_H
#define DMOT_VERSION_H
#define DMOT_VERSION "0.2.0"

#ifdef __cplusplus
extern "C"
{
#endif

    // Retrieves the current version of libdmotservices
    // @return          Current version of libdmotservices
    static inline const char *dmot_version_get_version(void)
    {
        return DMOT_VERSION;
    }

#ifdef __cplusplus
}
#endif
#endif // DMOT_VERSION_H
    