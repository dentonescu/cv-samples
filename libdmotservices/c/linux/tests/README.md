# libdmotservices (C/Linux Tests)

CMocka suites that exercise the Linux C implementation.

## Test files
- `test_log.c` — logging output and level checks.
- `test_math.c` — math helpers and random range behavior.
- `test_ringbuf.c` — ring buffer full/empty, wraparound, and type guards.
- `test_signals.c` — sine wave generation.
- `test_string.c` — string trimming and formatting.
- `test_version.c` — semantic version parsing.

## Run
```bash
make test
```

## Related
- Back to [Linux build](../README.md)
- Source modules: [../src/README.md](../src/README.md)
