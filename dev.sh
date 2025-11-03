#!/usr/bin/env bash
set -e

SUB_PROJECTS=()
while IFS= read -r git_dir; do
    SUB_PROJECTS+=("$(dirname "$git_dir")")
done < <(find . -maxdepth 2 -type d -name ".git")

print_usage_block() {
    printf '  %-18s %s\n' "$1" "$2"
}

usage() {
    {
        echo "Usage: $0 [options]"
        echo
        echo "Build, deploy, and run operations:"
        print_usage_block "--iface <iface>" "Select Wi-Fi interface (default: wlan0)."
        print_usage_block "--mock" "Toggle mock mode to skip hardware access."
        print_usage_block "--install-deps" "Install apt-based prerequisites."
        print_usage_block "--install-prj" "Install users, configs, and binaries."
        print_usage_block "--build" "Compile and link all sub-projects."
        print_usage_block "--run-tests" "Execute unit tests."
        print_usage_block "--run-examples" "Run available demo programs."
        echo
        echo "Git operations:"
        print_usage_block "--git-log" "Show git log for each sub-project."
        print_usage_block "--git-status" "Show short git status for each sub-project."
        print_usage_block "--git-push" "Push committed changes for each sub-project."
        echo
    } >&2
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

git_log() {
    for prj in "${SUB_PROJECTS[@]}"; do
        echo "== ${prj#./}"
        pushd "$prj" >/dev/null
        git log
        popd >/dev/null
    done
}

git_status() {
    for prj in "${SUB_PROJECTS[@]}"; do
        echo "== ${prj#./}"
        pushd "$prj" >/dev/null
        git status -sb
        popd >/dev/null
    done
}

git_push() {
    for prj in "${SUB_PROJECTS[@]}"; do
        echo "== ${prj#./}"
        pushd "$prj" >/dev/null
        git push
        popd >/dev/null
    done
}

if [ -z "$1" ]; then
    usage
fi

iface="wlan0"
mock=0
install_deps=0
install_prj=0
build=0
do_git_log=0
do_git_status=0
do_git_push=0
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
        --git-log)
            do_git_log=1
            ;;
        --git-status)
            do_git_status=1
            ;;
        --git-push)
            do_git_push=1
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

## git operations
[ ! "$do_git_log" -eq 1 ] || git_log
[ ! "$do_git_status" -eq 1 ] || git_status
[ ! "$do_git_push" -eq 1 ] || git_push

## build, deploy, and run operations
[ ! "$install_deps" -eq 1 ] || ensure_dependencies
[ ! "$build" -eq 1 ] || make clean all docs
[ ! "$install_prj" -eq 1 ] || make install
[ ! "$run_tests" -eq 1 ] || make test
[ ! "$run_examples" -eq 1 ] || WFQ_IFACE="$iface" WFQ_MOCK="$mock" make example-demo
