# libdmotservices (C/Linux)

Linux-focused build of **libdmotservices-c**, featuring signal utilities, logging, a generic ring buffer, and a text-mode equalizer demo.

## Project Summary
This module demonstrates my ability to:
- Deliver robust Linux command-line applications.
- Manage platform-specific builds using `make`.
- Apply modular software engineering principles in C.

## Highlights
- Static library: `libdmotservices_c.a`
- Unit tests: CMocka framework
- Equalizer demo: multithreaded, smooth visual feedback
- Ring buffer with typed helpers (C11 `_Generic` convenience macros)
- Designed for clarity and performance

## Build Prerequisites
```bash
sudo apt update
sudo apt install -y build-essential libcmocka-dev
```

## Build & Run
```bash
make clean all      # builds the static library
make tests          # builds test binaries
make examples       # builds example binaries
make test           # runs unit tests
make example-demo   # runs example demos
```

Note: C11 is recommended if you want the optional `_Generic` convenience macros in `ringbuf.h`. The core API remains C99-friendly.

## What this demonstrates
- Confident use of GNU toolchains and build automation.
- Test integration in native Linux environments.
- Hands-on understanding of signal processing and visualization in C.


## Screenshot

![C/Linux build screenshot](libdmotservices-c-linux.png)

## Examples

- [Example walkthroughs](examples/README.md)

## Subfolders
- [Public headers](include/README.md)
- [Source modules](src/README.md)
- [Tests](tests/README.md)
- [Screenshots](img/README.md)

## Related

- Developer notes: [NOTES.md](NOTES.md)
- Back to [libdmotservices (C)](../README.md)
- See how the equalizer is used in [WiFiEqu](../../../wifiequ/README.md)
