# libdmotservices.c.linux

Small, reusable C/Java utilities I keep around for demos, tests, and real projects.
This repo includes a C module that builds on Linux and ships a few visual examples,
including a terminal **equalizer** that displays channel power in dBm (ASCII bars).

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
- ASCII by default (works everywhere); optional Unicode skin later

## Quick start (C on Linux)

```bash
# from libdmotservices/c/linux
make clean all      # builds the static library
make examples       # builds example binaries
make test           # runs unit tests
make example-demo   # runs examples
```

## Screenshots

Screenshots of the library in action.

### make

![C/Linux build screenshot](libdmotservices-c-linux.png)

## equalizer

![Text-mode equalizer](img/equalizer.png)