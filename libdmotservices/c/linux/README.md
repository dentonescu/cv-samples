# libdmotservices (C/Linux)

Static C library for Linux, featuring a terminal equalizer and utility modules.

## Build prerequisites

```bash
# Ubuntu / Debian
sudo apt update
sudo apt install -y build-essential libcmocka-dev
```

## Highlights

- Clean C layout: `include/`, `src/`, `tests/`, `examples/`
- Builds a static library: `libdmotservices_c.a`
- Unit tests with CMocka
- Equalizer demo: raw vs smoothed signals; threaded sources
- ASCII by default (works everywhere)

## Build

```bash
# from libdmotservices/c/linux
make clean all      # builds the static library
make examples       # builds example binaries
make test           # runs unit tests
make example-demo   # runs examples
```

![C/Linux build screenshot](libdmotservices-c-linux.png)

## Examples

[Examples](examples/README.md)