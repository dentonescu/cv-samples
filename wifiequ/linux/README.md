# WiFiEqu (Linux)

Builds the Linux daemon (`wifiequd`) and terminal client examples.

## Build Prerequisites
```bash
sudo apt update
sudo apt install -y build-essential libcmocka-dev libmicrohttpd-dev libnl-3-dev libnl-genl-3-dev
```

## Build
```sh
make clean all              # or simply: make
```

## Install -- SystemD (optional)

Unit installs to `linux/etc/systemd/system/wifiequd.service` and makes sure the `wifiequ` user exists.

```sh
sudo setup-user             # create 'wifiequ' user and 'wifieq' group
sudo make install           # creates user and group and installs systemd unit and binary
sudo systemctl enable --now wifiequd
```

## Test & Demos
```sh
make tests                  # builds the tests
make examples               # builds the examples
make test                   # runs the unit tests
make example-demo           # runs the demos
```

Artifacts and logs are written under `bin/` and `logs/`.

## Development notes

- Notes I made during development of this project can be found here: [Development notes](NOTES.md)
- Live sampler uses nl80211 scan dump; aggregates **max dBm** per frequency and maps into configured bins. (*in progress*)

## Screenshots

SystemD service logs incoming JSON readings:
![Readings logged in the WiFiEqu daemon](img/wfq-daemon-readings.png)
