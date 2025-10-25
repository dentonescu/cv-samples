# NOTES

## Frequencies and signals

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

- [Wikipedia: List of WLAN channels](https://en.wikipedia.org/wiki/List_of_WLAN_channels): Channels and frequency ranges.
- [Wikipedia: Netlink](https://en.wikipedia.org/wiki/Netlink): AF_NETLINK socket family for IPC between kernel and userspace processes.

