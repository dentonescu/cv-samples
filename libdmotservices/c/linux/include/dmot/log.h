#ifndef DMOT_LOG_H
#define DMOT_LOG_H
#include <stdio.h>
#include "dmot/datatypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        DMOT_LOG_ERROR = 0,
        DMOT_LOG_WARN = 1,
        DMOT_LOG_INFO = 2,
        DMOT_LOG_DEBUG = 3
    } dmot_log_level;

    // Compares the supplied file pointer with the internal file pointer.
    // @param fp    File pointer.
    // @return      True if both file pointers are the same.
    bool dmot_log_cmp_fp(FILE *fp);

    // Provides the name of the log level enum.
    // @param level dmot_log_level. e.g. DMOT_LOG_ERROR, DMOT_LOG_WARN, DMOT_LOG_INFO, DMOT_LOG_DEBUG
    // @return String representation of the level: ERROR, WARN, INFO, DEBUG
    const char *dmot_log_level_name(dmot_log_level level);

    // Points to the stream to which logging data is written.
    // @param fp    File pointer.
    void dmot_log_set_file(FILE *fp);

    // Sets the lowest log level to be displayed: DEBUG, INFO, WARN, ERROR.
    // Messages with the given level and all those to the right of it will be displayed.
    // @param lvl   dmot_log_level enum.
    void dmot_log_set_level(dmot_log_level lvl);

    // Logs a message using the specified log level: DEBUG, INFO, WARN, ERROR.
    // Messages with the given level and all those to the right of it will be displayed.
    // @param lvl   dmot_log_level enum.
    // @param fmt   Format string.
    // @param ...   Accompanying variables required by fmt.
    // E.g. dmot_log(DMOT_LOG_ERROR, "Invalid text: %s", foo);
    void dmot_log(dmot_log_level lvl, const char *fmt, ...);

#define DMOT_LOGE(...) dmot_log(DMOT_LOG_ERROR, __VA_ARGS__)
#define DMOT_LOGW(...) dmot_log(DMOT_LOG_WARN, __VA_ARGS__)
#define DMOT_LOGI(...) dmot_log(DMOT_LOG_INFO, __VA_ARGS__)
#define DMOT_LOGD(...) dmot_log(DMOT_LOG_DEBUG, __VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif // DMOT_LOG_H