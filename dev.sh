#!/bin/sh
set -e

usage() {
    echo "Usage: $0 [--iface <wlan_interface>] [--mock] [--install-deps] [--install-prj] [--build] [--run-examples] [--run-tests]" >&2
    exit 1
}

ensure_dependencies() {
    if ! command -v apt-get >/dev/null 2>&1; then
        echo "Error: apt-get not found on this system; cannot install dependencies." >&2
        exit 1
    fi

    sudo apt-get update
    sudo apt-get install -y \
        build-essential \
        pkg-config \
        libcmocka-dev \
        libmicrohttpd-dev \
        libnl-3-dev \
        libnl-genl-3-dev \
    	openjdk-17-jdk \
    	python3-pytest
}

if [ -z "$1" ]; then
 usage
fi

iface="wlan0"
mock=0
install_deps=0
install_prj=0
build=0
run_examples=0
run_tests=0

while [ $# -gt 0 ]; do
    case "$1" in
        --iface)
            shift || { echo "Error: --iface requires an interface name." >&2; usage; }
            iface=$1
            case "$iface" in
                ""|-*)
                    echo "Error: --iface requires an interface name." >&2
                    usage
                    ;;
            esac
            ;;
        --mock)
            mock=1
            ;;
        --install-deps)
            install_deps=1
            ;;
        --install-prj)
            install_prj=1
            ;;
        --build)
            build=1
            ;;
        --run-tests)
            run_tests=1
            ;;
        --run-examples)
                run_examples=1
                ;;
        -h|--help)
            usage
            ;;
        *)
            echo "Unknown option: $1" >&2
            usage
            ;;
    esac
    shift
done

if [ -n "$iface" ]; then
    echo "Selected interface: $iface"
else
    echo "No interface specified."
fi

JAVA_HOME=${JAVA_HOME:-/usr/lib/jvm/java-17-openjdk-amd64}
if [ -d "$JAVA_HOME" ]; then
    export JAVA_HOME
else
    echo "Warning: JAVA_HOME ($JAVA_HOME) not found; skipping export." >&2
fi
echo "JAVA_HOME=$JAVA_HOME"

[ ! "$install_deps" -eq 1 ] || ensure_dependencies
[ ! "$build" -eq 1 ] || make clean all docs
[ ! "$install_prj" -eq 1 ] || make install
[ ! "$run_tests" -eq 1 ] || make test
[ ! "$run_examples" -eq 1 ] || WFQ_IFACE="$iface" WFQ_MOCK="$mock" make example-demo
