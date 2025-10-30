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
        DMOT_LOG_OFF = -1,
        DMOT_LOG_ERROR = 0,
        DMOT_LOG_WARN = 1,
        DMOT_LOG_INFO = 2,
        DMOT_LOG_DEBUG = 3
    } dmot_log_level;

    /**
     * @brief Checks whether logging currently targets a given file.
     *
     * @param[in] fp File pointer to compare with the active log stream.
     * @retval true The supplied file matches the configured stream.
     * @retval false The configured stream differs.
     */
    bool dmot_log_cmp_fp(FILE *fp);

    /**
     * @brief Returns a human-readable name for a log level.
     *
     * @param[in] level Log level to translate.
     * @return Constant string such as "ERROR", "WARN", "INFO", or "DEBUG".
     */
    const char *dmot_log_level_name(dmot_log_level level);

    /**
     * @brief Sets the output stream used for log messages.
     *
     * @param[in] fp Destination file pointer (defaults to `stderr` when null).
     */
    void dmot_log_set_file(FILE *fp);

    /**
     * @brief Configures the minimum log level that will be emitted.
     *
     * Messages at @p lvl and more severe will be logged.
     *
     * @param[in] lvl Threshold level to enable.
     */
    void dmot_log_set_level(dmot_log_level lvl);

    /**
     * @brief Emits a formatted log message at the requested level.
     *
     * Messages below the configured threshold are ignored.
     *
     * @param[in] lvl Severity level of the message.
     * @param[in] fmt `printf`-style format string.
     * @param[in] ... Variadic arguments substituted into @p fmt.
     */
    void dmot_log(dmot_log_level lvl, const char *fmt, ...);

#define DMOT_LOGE(...) dmot_log(DMOT_LOG_ERROR, __VA_ARGS__)
#define DMOT_LOGW(...) dmot_log(DMOT_LOG_WARN, __VA_ARGS__)
#define DMOT_LOGI(...) dmot_log(DMOT_LOG_INFO, __VA_ARGS__)
#define DMOT_LOGD(...) dmot_log(DMOT_LOG_DEBUG, __VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif // DMOT_LOG_H
