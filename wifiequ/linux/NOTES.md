# NOTES

### dBm

dBm is a logarithmic unit that expresses power relative to 1 milliwatt (mW).
- 0 dBm = 1 mW
- 10 dBm = 10 mW
- 20 dBm = 100 mW
- -10 dbM = 0.1 mW

$$
dBm = 10 \cdot \log_{10}\left(\frac{P}{1\ \text{mW}}\right)
$$

### wifiequd JSON example
```json
{
    "timestamp_ms": 1734760118123,
    "readings": [
        {"chan": 1, "dbm": -25.0},
        {"chan": 2, "dbm": -20.0},
        {"chan": 7, "dbm": -50.3}
    ]
}

```

### Frequencies and signals

```bash
ip a
```

**wlan0** is my wireless interface.

```bash
sudo iw dev wlan0 scan | less
```

![BSS details page 1 example](img/wfq-bss-01.png)

The relevant rows for WiFiEqu are **freq** and **signal**.

```bash
sudo iw dev wlan0 scan | grep -E 'freq:|signal:'
```

![Frequencies and signals example 1](img/wfq-freq-signal-01.png)

Monitoring the signals and frequencies:

```bash
sudo watch -n 1 "echo 'freq\tsignal' && iw dev wlan0 scan | awk '/freq:/{f=\$2}/signal:/{print f,"\t",\$2}'"
```

![Frequencies and signals example 2](img/wfq-freq-signal-02.png)

## Reading materials

- [GNU Libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/)
- [Linux Wireless documentation](https://wireless.docs.kernel.org/en/latest/en/developers/documentation/nl80211.html)
- [Stackoverflow: How do the nl80211 library & cfg80211 work?](https://stackoverflow.com/questions/21456235/how-do-the-nl80211-library-cfg80211-work): Overview of the nl80211 library
- [Wikipedia: List of WLAN channels](https://en.wikipedia.org/wiki/List_of_WLAN_channels): Channels and frequency ranges.
- [Wikipedia: Netlink](https://en.wikipedia.org/wiki/Netlink): AF_NETLINK socket family for IPC between kernel and userspace processes.

## Related

- Back to [WiFiEqu (Linux)](README.md)
- [Architecture overview](ARCHITECTURE.md)
- [Examples](examples/README.md)
