# libdmotservices (C)

**libdmotservices-c** provides portable C utilities designed to be simple, reusable, and illustrative.  
It includes a logging subsystem, signal generation helpers, and a terminal equalizer demo.

### Operating Systems

- [Linux](linux/README.md): C library for Linux


### Project Summary
This module demonstrates my ability to:
- Build modular, dependency-light C libraries with a clear include/src/tests structure.
- Integrate automated builds and tests using Make and CMocka.
- Create practical console-based visualization and signal-processing demos.


### Structure
| Path | Description |
|------|--------------|
| `include/` | Public headers for the C library. |
| `src/` | Implementation source files. |
| `tests/` | Unit tests using CMocka. |
| `examples/` | Standalone demo programs (signal generator, UI equalizer). |

---
### Build & Run
```bash
make clean all      # builds the static library
make tests          # builds test binaries
make examples       # builds example binaries
make test           # runs unit tests
make example-demo   # runs example demos
```

### Highlights
- Static library output: `libdmotservices_c.a`
- CMocka-based test suite
- ANSI terminal visualization (equalizer) with per-channel labelling and auto-hiding of unused bins
- Clean separation of headers, sources, and examples

### What this demonstrates
- Low-level systems programming and structured C design.
- Test-driven development and cross-platform build practices.
- Effective CLI-based visualization and data presentation.

---

## Equalizer at a glance

- `examples/ex_ui.c` renders both raw and smoothed signals.
- `dmot_ui_equalizer_set_channel_name` lets you assign short labels (e.g. Wi-Fi channels).
- `dmot_ui_equalizer_hide_chans_without_labels` keeps the output focused by skipping unnamed channels.
- The renderer only clears rows it actually printed, so it plays nicely alongside other console output.

This component powers the live Wi-Fi equalizer in the companion WiFiEqu project.
