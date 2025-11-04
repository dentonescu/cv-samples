# NOTES

Developer notes for the Linux-focused build of `libdmotservices-c`. These highlight platform specifics that differ from the generic C module.

## Current File Layout
```
linux/
├─ README.md                     # Platform-specific build guidance
├─ NOTES.md                      # Developer roadmap (this file)
├─ Makefile
├─ include/dmot/*.h              # Installed header set
├─ src/
│  ├─ datatypes/datatypes.c
│  ├─ math/{math.c, mathx.d, signals.c}
│  ├─ string/{string.c, stringx.d}
│  ├─ time/{time.c, timex.d}
│  └─ util/log.c
├─ tests/
│  ├─ test_log.c
│  ├─ test_math.c
│  ├─ test_signals.c
│  ├─ test_string.c
│  └─ test_version.c
├─ examples/
│  ├─ README.md
│  ├─ ex_signals.c
│  ├─ ex_time.c
│  └─ ex_ui.c
├─ img/equalizer.png
├─ libdmotservices-c-linux.png
├─ logs/                         # Sample output (gitignored contents)
└─ bin/ / lib/ / tests/ artifacts (generated at build time)
```

## Implementation Plan
1. **Build system polish**  
   - Keep the platform `Makefile` thin: delegate most compilation flags to the parent `c/` folder to minimise duplication.  
   - Offer `make sanitize`, `make coverage`, and `make bench` targets for richer CI runs.  
   - Document GCC/Clang minimum versions and feature flags (pthread, rt) in the README.
2. **Integration with WiFiEqu**  
   - Ensure the equalizer demo (`examples/ex_ui.c`) mirrors WiFiEqu’s refresh cadence and channel labelling, making it a faithful stand-alone showcase.  
   - Provide shared header to translate configuration entries into equalizer labels so WiFiEqu and the demos stay aligned.  
   - Record real sampler output from WiFiEqu mock/live runs and store as fixtures for regression tests.
3. **Packaging**  
   - `make install` should copy headers to `/usr/local/include/dmot/` and the static library to `/usr/local/lib/`; include an uninstall target.  
   - Produce a `.pc` file for pkg-config to help downstream projects link the library easily.  
   - Update the screenshot and docs whenever CLI visuals change.
4. **Testing & QA**  
   - Expand CMocka tests to cover platform-specific helpers (threading, timing).  
   - Add shell scripts that run demos under `timeout` to prevent CI hangs.  
   - Verify builds on ARM (Raspberry Pi) and note any divergences.

## Domain Notes
- Threads: the equalizer demo uses pthreads; ensure clean shutdowns and join semantics when integrating with other apps.  
- Terminal handling: respect `$TERM` capabilities; fall back to minimal output when ANSI control codes are not supported.  
- Logging destination: keep logs under `logs/` with rotation-ready file naming (`YYYYMMDD-hhmm`).  
- Dist layout: `bin/`, `lib/`, `include/`, and `logs/` mimic the structure expected by WiFiEqu to simplify sharing artifacts.

## Implementation Timeline
- **2025-10-12 – 2025-10-18: Linux build bring-up**  
  Platform Makefiles and ignore rules stabilised the Linux build, while console sizing and thread-aware equalizer demos proved the UI on real terminals.
- **2025-10-19 – 2025-10-26: Packaging & API refinements**  
  Library outputs began landing in `lib/`, channel labelling and hiding shipped, and redraw correctness ensured clean terminal updates—perfect timing for WiFiEqu mock/live demos.
- **2025-10-29 – 2025-11-02: Versioning alignment**  
  Semantic version helpers and attribute normalisation kept the Linux artefacts in sync with the rest of libdmotservices ahead of downstream packaging.

## Testing Checklist
- `make clean all`, `make test`, `make example-demo`, and `make dist` run without warnings.  
- Sanitiser builds (`make sanitize`) complete cleanly.  
- Packaging scripts (`make install`, `make uninstall`) work on fresh systems.

## Related Documentation
- Module overview: [README.md](README.md)  
- Parent C notes: [../NOTES.md](../NOTES.md)  
- Project-wide context: [../../NOTES.md](../../NOTES.md)
