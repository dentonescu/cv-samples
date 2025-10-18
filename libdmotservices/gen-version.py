#!/usr/bin/env python3
import pathlib, sys
root = pathlib.Path(__file__).resolve().cwd()
ver = (root/"VERSION").read_text().strip()

# C header
for c_os in [ "linux" ]:    # C for now, but maybe I'll make a Windows C part in the future
    cdir = root/"c"/c_os/"include"/"dmot"
    cdir.mkdir(parents=True, exist_ok=True)
    (root/"c"/c_os/"include"/"dmot"/"version.h").write_text(
        f'#ifndef DMOT_VERSION_H\n#define DMOT_VERSION_H\n#define DMOT_VERSION "{ver}"\n#endif\n'
    )

# Java
## No action necessary for Java. The Ant orchestrator will get the version from the top-level VERSION file.

print(ver)
