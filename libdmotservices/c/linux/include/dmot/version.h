
#ifndef DMOT_VERSION_H
#define DMOT_VERSION_H
#define DMOT_VERSION "0.3.0"

#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Retrieves the full semantic version string of libdmotservices.
     *
     * @return Null-terminated version string.
     */
    static inline const char *dmot_version_get_version(void)
    {
        return DMOT_VERSION;
    }
    
    /**
     * @brief Parses an individual component from the version string.
     *
     * @param[in] component_index Zero-based component index.
     * @return Parsed component value, or `0` if missing.
     */
    static inline unsigned int dmot_version_get_version_component(unsigned int component_index)
    {
        const char *cursor = dmot_version_get_version();
        while (component_index > 0U)
        {
            const char *dot = strchr(cursor, '.');
            if (!dot)
                return 0U;
            cursor = dot + 1;
            component_index--;
        }

        unsigned int value = 0U;
        while (*cursor != '\0' && *cursor != '.')
        {
            if (*cursor < '0' || *cursor > '9')
                break;
            value = (value * 10U) + (unsigned int)(*cursor - '0');
            ++cursor;
        }
        return value;
    }

    /**
     * @brief Returns the major component of the libdmotservices version.
     *
     * @return Major version number.
     */
    static inline unsigned int dmot_version_get_version_major(void)
    {
        return dmot_version_get_version_component(0U);
    }

    /**
     * @brief Returns the minor component of the libdmotservices version.
     *
     * @return Minor version number.
     */
    static inline unsigned int dmot_version_get_version_minor(void)
    {
        return dmot_version_get_version_component(1U);
    }

    /**
     * @brief Returns the patch component of the libdmotservices version.
     *
     * @return Patch version number.
     */
    static inline unsigned int dmot_version_get_version_patch(void)
    {
        return dmot_version_get_version_component(2U);
    }

#ifdef __cplusplus
}
#endif
#endif // DMOT_VERSION_H
