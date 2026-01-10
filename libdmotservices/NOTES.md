# NOTES

Developer notes for **libdmotservices** — the umbrella project that houses the reusable C and Java utilities backing several samples (WiFiEqu, CLI demos, servlet helpers).

## Current stage
- Cross-language reboot is healthy: C and Java modules ship stable APIs, the Python helpers are live, and JavaScript utilities just landed for pkixwebadm.
- The C module now includes a generic ring buffer with typed wrappers and C11-friendly convenience macros.
- Active work tracks language parity (Python + JS ergonomics) and shared tooling so version bumps, Make targets, and doc refreshes stay aligned across the tree.

## Current File Layout
```
libdmotservices/
├─ README.md                     # Project overview and integration notes
├─ NOTES.md                      # Cross-language developer log
├─ VERSION                       # Library release tag
├─ Makefile                      # Orchestrates C and Java builds/tests
├─ gen-version.py                # Shared VERSION bumping helper
├─ .gitattributes / .gitignore
├─ dist/                         # Published artifacts (static libs, jars, demos)
├─ js/
│  ├─ README.md / Makefile / package.json
│  ├─ src/*.js                   # Browser helpers (logger, FormData utilities)
│  ├─ dist/index.js              # Concatenated bundle (generated)
│  └─ tests/*.test.js            # node --test suites
├─ python/
│  ├─ README.md / tests/
│  └─ libdmotservices/{args.py,string.py}
├─ c/
│  ├─ README.md / NOTES.md
│  ├─ Makefile
│  ├─ .vscode/
│  └─ linux/
│     ├─ README.md / NOTES.md
│     ├─ Makefile
│     ├─ include/dmot/*.h        # Public headers (math, ring buffer, string, time, ...)
│     ├─ src/
│     │  ├─ datatypes/datatypes.c
│     │  ├─ datatypes/ringbuf.c
│     │  ├─ math/{math.c, signals.c, mathx.d}
│     │  ├─ string/{string.c, stringx.d}
│     │  ├─ time/{time.c, timex.d}
│     │  └─ util/log.c
│     ├─ tests/                  # CMocka suites (test_math.c, test_ringbuf.c, ...)
│     ├─ examples/               # Sample programs (ex_signals.c, ...)
│     └─ img/equalizer.png
├─ java/
│  ├─ README.md / NOTES.md
│  ├─ build.xml / build.sh / build.cmd
│  ├─ log4j2.xml
│  ├─ mock/                      # Test doubles
│  ├─ dmot-core/
│  │  ├─ README.md / NOTES.md
│  │  ├─ build.xml / pom.xml
│  │  ├─ dep/jars/json-20231013.jar
│  │  └─ src/
│  │     ├─ main/java/dev/dmotservices/**  # Core API & utilities
│  │     └─ test/java/dev/dmotservices/**  # JUnit tests
│  └─ dmot-servlet-javax/
│     ├─ README.md / NOTES.md
│     ├─ build.xml
│     └─ src/
│        ├─ main/java/dev/dmotservices/servlet/**  # Servlet adapters
│        └─ test/java/dev/dmotservices/servlet/**  # Tests
├─ libdmotservices.logo
└─ libdmotservices.png
```

## Project Snapshot
- Shared assets: top-level `Makefile` syncs version numbers from `VERSION`, kicks off language-specific builds, runs tests, and copies artifacts into `dist/`.  
- Cross-project usage: the equalizer renderer feeds WiFiEqu demos; Java utilities support planned Spring/Terraglean services.  
- Documentation: language-specific READMEs (`c/`, `java/`) focus on build/run, while this file tracks overarching plans and design choices.

## Development Plan _(currently executing: Step 2 – Language parity & shared tooling)_
1. **Versioning & release hygiene**  
   - Keep `gen-version.py` as the single source of truth for Git metadata; call it from each subproject’s `Makefile`.  
   - Automate changelog fragments per language, then roll them up into a root `CHANGELOG.md` when publishing.  
   - Add release packaging target that produces language-specific tarballs plus a unified manifest.
2. **Language parity**  
   - Ensure C, Java, and Python expose comparable modules (logging/timing/equalizer abstractions for native code; CLI/helpers for Python).  
   - Document the shared conceptual model (signal channels, logging levels) so other languages can join later.  
   - Evaluate generating bindings (e.g., C headers → Java JNI/Python cffi) only after the APIs stabilise.
3. **Testing discipline**  
   - Run `make test` (root) to execute C CMocka suites and Java JUnit suites sequentially; surface combined coverage metrics.  
   - Provide smoke demos (`make example-demo`) that tie into consumers (WiFiEqu) for regression checks.  
   - Keep the Python pytest suites aligned with downstream consumers (pkixwebadm CLI, FastAPI app).
   - Add pre-commit hooks or CI steps to guard header/ABI compatibility.
4. **Documentation & samples**  
   - Expand language-specific NOTES to capture design context (see `c/NOTES.md`, `java/NOTES.md`).  
   - Highlight cross-project integration examples (e.g., WiFiEqu CLI screenshot) and link them from this file.  
   - Consider publishing API references (Doxygen / Javadoc) under `docs/`.
5. **Future languages**  
   - Keep structure ready for additional folders (`typescript/`, etc.).  
   - Document expectations (tests, examples, packaging) so new languages plug in consistently.

## Domain Notes
- Root `Makefile` orchestrates per-language builds via `$(MAKE) -C <dir>`—avoid adding language-specific tooling here; keep those within subdirectories.  
- Version propagation relies on text replacements; ensure newline conventions remain consistent (LF).  
- When updating shared assets (logos, docs), keep file paths stable to avoid breaking downstream references (e.g., WiFiEqu docs embed the equalizer image).
- Python helpers are packaged via editable installs (`python -m pip install -e .`) so sibling projects can depend on them without extra path hacks.
- The C ring buffer keeps `_Generic` helpers optional (C11) so the base API remains portable.

## Implementation Timeline
- **2021-10 – 2023-04: Early Java library era**  
  The original Maven project took shape, groupIds were refined, and Jakarta alignment experiments landed before the codebase paused for a while.
- **2025-10-11 – 2025-10-15: Multi-language reboot**  
  The project relaunched with versioned Java modules, Ant orchestration, and a split between `dmot-core` and `dmot-servlet-javax`. Documentation and screenshots reintroduced the library, while the C side surfaced with Makefiles, unit tests, and sine-wave generators.
- **2025-10-14 – 2025-10-18: Equalizer & build orchestration**  
  UI work introduced the equalizer rendering pipeline, threading demos, and console sizing, while master Makefile and version header tooling unified builds across languages.
- **2025-10-19 – 2025-10-26: Feature expansion & releases**  
  Timestamp helpers, string utilities, and labelled equalizer channels matured the C API, leading to a trio of releases that delivered channel hiding, smoothing, and stable redraws in time for WiFiEqu integration.
- **2025-10-29 – 2025-11-02: Versioning parity & polish**  
  Cross-language semantic version helpers, documentation refreshes, and build corrections cemented the current state, keeping the shared library ready for downstream consumers.
- **2025-11-06 – present: Python helper rollout**  
  Introduced the Python package to share CLI and path utilities with pkixwebadm. Added pytest coverage and the `print_help_all` helper, replacing ad-hoc code in downstream projects.

## Testing Checklist
- `make clean all` and `make test` succeed at the root on a clean machine.  
- Language-specific tests/demos continue to run in isolation (`make -C c test`, `make -C java test`).  
- Dist artifacts (`dist/*.tar.gz`, `.jar`) carry the correct semantic versions and manifest metadata.

## Related Documentation
- Project overview: [README.md](README.md)  
- C modules: [c/README.md](c/README.md)  
- C developer notes: [c/NOTES.md](c/NOTES.md)  
- Java modules: [java/README.md](java/README.md)  
- Java developer notes: [java/NOTES.md](java/NOTES.md)
