#!/usr/bin/env python3
import pathlib, sys
root = pathlib.Path(__file__).resolve().parent
ver = (root/"VERSION").read_text().strip()

# Linux/C
cdir = root/"linux"/"include"/"wfq"
cdir.mkdir(parents=True, exist_ok=True)
(cdir/"version.h").write_text(f'''
#ifndef WFQ_VERSION_H
#define WFQ_VERSION_H
#define WFQ_VERSION "{ver}"

#include <string.h>

/** @file wfq/version.h
 *  @brief Version metadata helpers for WiFiEqu.
 */

#ifdef __cplusplus
extern "C"
{{
#endif

    /**
     * @brief Retrieves the full semantic version string of WiFiEqu.
     *
     * @return Null-terminated version string.
     */
    static inline const char *wfq_version_get_version(void)
    {{
        return WFQ_VERSION;
    }}
    
    /**
     * @brief Parses a single numeric component from the version string.
     *
     * @param[in] component_index Zero-based index of the component (`0` = major, `1` = minor, ...).
     * @return Parsed component value, or `0` if the component is absent.
     */
    static inline unsigned int wfq_version_get_version_component(unsigned int component_index)
    {{
        const char *cursor = wfq_version_get_version();
        while (component_index > 0U)
        {{
            const char *dot = strchr(cursor, '.');
            if (!dot)
                return 0U;
            cursor = dot + 1;
            component_index--;
        }}

        unsigned int value = 0U;
        while (*cursor != '\\0' && *cursor != '.')
        {{
            if (*cursor < '0' || *cursor > '9')
                break;
            value = (value * 10U) + (unsigned int)(*cursor - '0');
            ++cursor;
        }}
        return value;
    }}

    /**
     * @brief Returns the major component of the WiFiEqu version.
     *
     * @return Major version number.
     */
    static inline unsigned int wfq_version_get_version_major(void)
    {{
        return wfq_version_get_version_component(0U);
    }}

    /**
     * @brief Returns the minor component of the WiFiEqu version.
     *
     * @return Minor version number.
     */
    static inline unsigned int wfq_version_get_version_minor(void)
    {{
        return wfq_version_get_version_component(1U);
    }}

    /**
     * @brief Returns the patch component of the WiFiEqu version.
     *
     * @return Patch version number.
     */
    static inline unsigned int wfq_version_get_version_patch(void)
    {{
        return wfq_version_get_version_component(2U);
    }}

#ifdef __cplusplus
}}
#endif
#endif // WFQ_VERSION_H
''')

# Windows/C#
## TO-DO

# Angular
## TOD-DO

print(ver)
