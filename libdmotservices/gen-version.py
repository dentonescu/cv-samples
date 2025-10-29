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

    // Retrieves the current version of libdmotservices
    // @return          Current version of libdmotservices
    static inline const char *dmot_version_get_version(void)
    {{
        return DMOT_VERSION;
    }}
    
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

    static inline unsigned int dmot_version_get_version_major(void)
    {{
        return dmot_version_get_version_component(0U);
    }}

    static inline unsigned int dmot_version_get_version_minor(void)
    {{
        return dmot_version_get_version_component(1U);
    }}

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

public class Versioning {{

    private static final String VERSION = "{ver}";
    private static final String BASE_VERSION = VERSION.split("-", 2)[0];
    private static final String[] VERSION_PARTS = BASE_VERSION.split("\\\\.");

    public static String getVersion() {{
        return VERSION;
    }}

    public static int getMajor() {{
        return Integer.parseInt(VERSION_PARTS[0]);
    }}

    public static int getMinor() {{
        return (VERSION_PARTS.length > 1) ? Integer.parseInt(VERSION_PARTS[1]) : 0;
    }}

    public static int getPatch() {{
        return (VERSION_PARTS.length > 2) ? Integer.parseInt(VERSION_PARTS[2]) : 0;
    }}
}}
''')

print(ver)
