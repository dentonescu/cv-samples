#!/usr/bin/env python3
import pathlib, sys
root = pathlib.Path(__file__).resolve().cwd()
ver = (root/"VERSION").read_text().strip()

# Linux/C
cdir = root/"linux"/"include"/"wfq"
cdir.mkdir(parents=True, exist_ok=True)
(root/"linux"/"include"/"wfq"/"version.h").write_text(
    f'#ifndef WFQ_VERSION_H\n#define WFQ_VERSION_H\n#define WFQ_VERSION "{ver}"\n#endif\n')

# Windows/C#
## TO-DO

# Angular
## TOD-DO

print(ver)