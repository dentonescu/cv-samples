#!/usr/bin/env python3
import pathlib, sys
root = pathlib.Path(__file__).resolve().cwd()
ver = (root/"VERSION").read_text().strip()

# Linux/C
cdir = root/"linux"/"include"/"wfq"
cdir.mkdir(parents=True, exist_ok=True)
(root/"linux"/"include"/"wfq"/"version.h").write_text(f'''
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