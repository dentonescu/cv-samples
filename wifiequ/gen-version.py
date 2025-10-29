#!/usr/bin/env python3
import pathlib, sys
root = pathlib.Path(__file__).resolve().cwd()
ver = (root/"VERSION").read_text().strip()

# Linux/C
cdir = root/"linux"/"include"/"wfq"
cdir.mkdir(parents=True, exist_ok=True)
(cdir/"version.h").write_text(f'''
#ifndef WFQ_VERSION_H
#define WFQ_VERSION_H
#define WFQ_VERSION "{ver}"

#ifdef __cplusplus
extern "C"
{{
#endif

    // Retrieves the current version of wifique
    // @return          Current version of wifique
    static inline const char *wfq_version_get_version(void)
    {{
        return WFQ_VERSION;
    }}
    
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

    static inline unsigned int wfq_version_get_version_major(void)
    {{
        return wfq_version_get_version_component(0U);
    }}

    static inline unsigned int wfq_version_get_version_minor(void)
    {{
        return wfq_version_get_version_component(1U);
    }}

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
