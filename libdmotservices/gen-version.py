#!/usr/bin/env python3
import pathlib, sys
root = pathlib.Path(__file__).resolve().cwd()
ver = (root/"VERSION").read_text().strip()

# C header
cdir = root/"c"/"linux"/"include"/"dmot"
cdir.mkdir(parents=True, exist_ok=True)
(cdir/"version.h").write_text(f'''
#ifndef DMOT_VERSION_H
#define DMOT_VERSION_H
#define DMOT_VERSION "{ver}"

#include <string.h>

#ifdef __cplusplus
extern "C"
{{
#endif

    /**
     * @brief Retrieves the full semantic version string of libdmotservices.
     *
     * @return Null-terminated version string.
     */
    static inline const char *dmot_version_get_version(void)
    {{
        return DMOT_VERSION;
    }}
    
    /**
     * @brief Parses an individual component from the version string.
     *
     * @param[in] component_index Zero-based component index.
     * @return Parsed component value, or `0` if missing.
     */
    static inline unsigned int dmot_version_get_version_component(unsigned int component_index)
    {{
        const char *cursor = dmot_version_get_version();
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
     * @brief Returns the major component of the libdmotservices version.
     *
     * @return Major version number.
     */
    static inline unsigned int dmot_version_get_version_major(void)
    {{
        return dmot_version_get_version_component(0U);
    }}

    /**
     * @brief Returns the minor component of the libdmotservices version.
     *
     * @return Minor version number.
     */
    static inline unsigned int dmot_version_get_version_minor(void)
    {{
        return dmot_version_get_version_component(1U);
    }}

    /**
     * @brief Returns the patch component of the libdmotservices version.
     *
     * @return Patch version number.
     */
    static inline unsigned int dmot_version_get_version_patch(void)
    {{
        return dmot_version_get_version_component(2U);
    }}

#ifdef __cplusplus
}}
#endif
#endif // DMOT_VERSION_H
''')

# Java
java_dir = root/"java"/"dmot-core"/"src"/"main"/"java"/"dev"/"dmotservices"/"versioning"
java_dir.mkdir(parents=True, exist_ok=True)
(java_dir/"Versioning.java").write_text(f'''
package dev.dmotservices.versioning;

/**
 * Provides access to the current libdmotservices version at runtime.
 */
public class Versioning {{

    private static final String VERSION = "{ver}";
    private static final String BASE_VERSION = VERSION.split("-", 2)[0];
    private static final String[] VERSION_PARTS = BASE_VERSION.split("\\\\.");

    /**
     * Returns the full semantic version string.
     *
     * @return The raw version string, including any pre-release suffix.
     */
    public static String getVersion() {{
        return VERSION;
    }}

    /**
     * Returns the major component of the version.
     *
     * @return Major version number.
     */
    public static int getMajor() {{
        return Integer.parseInt(VERSION_PARTS[0]);
    }}

    /**
     * Returns the minor component of the version.
     *
     * @return Minor version number, or {{@code 0}} when absent.
     */
    public static int getMinor() {{
        return (VERSION_PARTS.length > 1) ? Integer.parseInt(VERSION_PARTS[1]) : 0;
    }}

    /**
     * Returns the patch component of the version.
     *
     * @return Patch version number, or {{@code 0}} when absent.
     */
    public static int getPatch() {{
        return (VERSION_PARTS.length > 2) ? Integer.parseInt(VERSION_PARTS[2]) : 0;
    }}
}}
''')

print(ver)
