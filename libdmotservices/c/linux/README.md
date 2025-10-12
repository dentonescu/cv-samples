# libdmotservices.c.linux

Minimal C library for Linux with examples and unit tests.

## Build prerequisites

```bash
# Ubuntu / Debian
sudo apt update
sudo apt install -y build-essential libcmocka-dev
```

## Build the static library
```bash
make
```

## Erase previous builds
```bash
make clean
```

## Build all run tests and examples
```bash
make clean all test example-demo
```

### Screenshot
![C/Linux build screenshot](libdmotservices-c-linux.png)

