# syntax=docker/dockerfile:1

## Stage 1: heavy image to compile and produce binaries
FROM debian:bookworm-slim AS builder

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        pkg-config \
        libcmocka-dev \
        libmicrohttpd-dev \
        libnl-3-dev \
        libnl-genl-3-dev \
        python3 \
        python3-pip \
        openssl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN make -C libdmotservices/c/linux clean all && \
    make -C wifiequ clean all


## Stage 2: runtime image with just the binaries and runtime dependencies
FROM debian:bookworm-slim

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        libmicrohttpd12 \
        libnl-3-200 \
        libnl-genl-3-200 \
        openssl \
    && rm -rf /var/lib/apt/lists/*

COPY docker/wifiequ-entrypoint.sh /usr/local/bin/wifiequ-entrypoint.sh
RUN chmod +x /usr/local/bin/wifiequ-entrypoint.sh

COPY --from=builder /src/wifiequ/linux/bin/wifiequd /usr/local/bin/wifiequd
COPY --from=builder /src/wifiequ/linux/etc/wifiequd.conf /etc/wifiequd.conf

ENV WFQ_MOCK=1 \
    WFQ_IFACE=wlan0 \
    WFQ_CONFIG_PATH=/etc/wifiequd.conf

EXPOSE 8080

ENTRYPOINT ["/usr/local/bin/wifiequ-entrypoint.sh"]
CMD []
