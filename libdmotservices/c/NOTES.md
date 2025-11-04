# NOTES

Developer notes for the C library shared across demos and WiFiEqu. Keep these aligned with the platform-specific builds (see `linux/NOTES.md` for host-specific details).

## Current File Layout
```
c/
├─ README.md                     # C track overview
├─ NOTES.md                      # Developer roadmap (this file)
├─ Makefile                      # Root C build orchestrator
├─ .gitignore
├─ .vscode/
│  ├─ c_cpp_properties.json
│  └─ settings.json
└─ linux/                        # Linux implementation target
   ├─ README.md / NOTES.md
   ├─ Makefile
   ├─ include/dmot/*.h           # Public headers
   ├─ src/                       # Library source modules
   │  ├─ datatypes/datatypes.c
   │  ├─ math/{math.c, signals.c}
   │  ├─ string/{string.c, stringx.d}
   │  ├─ time/{time.c, timex.d}
   │  └─ util/log.c
   ├─ tests/                     # CMocka suites
   ├─ examples/                  # Example programs (ex_signals.c, …)
   ├─ img/equalizer.png
   └─ libdmotservices-c-linux.png
```

## Implementation Plan
1. **Core library polish**  
   - Maintain clean separation between `include/` and `src/`; avoid leaking internal headers.  
   - Expand logging helpers with structured output (severity, timestamp, component).  
   - Add configurable smoothing algorithms for the equalizer (e.g., exponential moving average vs. simple roll).
2. **Testing coverage**  
   - Grow the CMocka suite to cover ring buffer behaviours, logging edge cases, and equalizer bounds.  
   - Provide fixture-based tests that mirror inputs from WiFiEqu (channel labels, hidden bins).  
   - Integrate sanitizers (`-fsanitize=address,undefined`) under an optional `make test-sanitize` target.
3. **Examples & demos**  
   - Keep `examples/` showcasing realistic signal generation; document how each ties back to WiFiEqu scenarios.  
   - Add a headless example that outputs JSON for quick integration checks.  
   - Record asciinema sessions to embed in docs.
4. **Packaging**  
   - Ensure `make dist` produces a versioned tarball with headers, static library, and example sources.  
   - Document how dependent projects link against the library (pkg-config stub or `cmake` config).  
   - Validate that archives extract cleanly on macOS/Linux (no absolute paths).
5. **Cross-project reuse**  
   - Track consumer expectations (WiFiEqu, future tools) so API changes remain backward compatible.  
   - Keep changelog entries describing new structs/functions and deprecation windows.

## Domain Notes
- Equalizer API: `dmot_ui_equalizer_*` functions expect pre-normalised channel bins; maintain invariants (channel count <= `DMOT_UI_EQUALIZER_MAX_CHAN`, dBm ranges).  
- Signal utilities: smoothing helpers and channel mapping should guard against negative frequencies and clamps; document expected behaviour on invalid input.  
- Logging macros should default to ANSI-safe output; guard optional colour codes behind feature flags for portability.

## Implementation Timeline
- **2025-10-12 – 2025-10-15: Foundations**  
  The initial C port introduced Makefiles, a sine-wave generator, and the CMocka harness. Logos and documentation quickly followed to align with the refreshed branding.
- **2025-10-14 – 2025-10-18: Equalizer engine**  
  UI scaffolding rapidly evolved into threaded, colourised equalizers with terminal sizing and deterministic seeding, while the master Makefile kept module builds consistent.
- **2025-10-19 – 2025-10-26: Library maturation**  
  Timestamp utilities, namespace-safe string helpers, and labelled channels shipped ahead of WiFiEqu integration. Successive releases added channel hiding, redraw cleanup, and high-channel-count support.
- **2025-10-29 – 2025-11-02: Versioning & polish**  
  Semantic version helpers, documentation sweeps, and naming clean-ups rounded out the current API surface, keeping the library ready for downstream packaging.

## Testing Checklist
- `make clean all`, `make tests`, `make test`, and `make example-demo` succeed in CI.  
- Headers compile under `-Wall -Wextra -Werror` with GCC and Clang.  
- Sanitiser runs (where available) complete without leaks or UB findings.

## Related Documentation
- Module overview: [README.md](README.md)  
- Linux packaging details: [linux/README.md](linux/README.md)  
- Project-wide notes: [../NOTES.md](../NOTES.md)
