# libdmotservices (C)

**libdmotservices-c** provides portable C utilities designed to be simple, reusable, and illustrative.  
It includes a logging subsystem, signal generation helpers, and a terminal equalizer demo.


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
- ANSI terminal visualization (equalizer)
- Clean separation of headers, sources, and examples

### What this demonstrates
- Low-level systems programming and structured C design.
- Test-driven development and cross-platform build practices.
- Effective CLI-based visualization and data presentation.
