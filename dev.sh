#!/usr/bin/env bash
set -e
SCRIPT_DIR="$(cd -- "$(dirname -- "$0")" && pwd)"

SUB_PROJECTS=()
while IFS= read -r git_dir; do
    project_dir="$(dirname "$git_dir")"
    if [ "$project_dir" != "$SCRIPT_DIR" ]; then
        SUB_PROJECTS+=("$project_dir")
    fi
done < <(find "$SCRIPT_DIR" -maxdepth 2 -type d -name ".git")

ANSI_GREEN='\033[32m'
ANSI_RED='\033[31m'
ANSI_RESET='\033[0m'

##
## Help
##

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
        print_usage_block "--git-diff" "Show Git differences for each sub-project."
        print_usage_block "--git-log" "Show Git log for each sub-project."
        print_usage_block "--git-push" "Push committed changes for each sub-project."
        print_usage_block "--git-status" "Show short Git status for each sub-project."
        echo
    } >&2
    exit 1
}

##
## Dependencies
##


ensure_dependencies() {
    if ! command -v apt-get >/dev/null 2>&1; then
        echo "Error: apt-get not found on this system; cannot install dependencies." >&2
        exit 1
    fi

    sudo apt-get update
    for pkg in build-essential pkg-config libcmocka-dev libmicrohttpd-dev \
        libnl-3-dev libnl-genl-3-dev openjdk-17-jdk nodejs python3-bcrypt python3-fastapi \
        python3-jinja2 python3-multipart python3-pip python3-pydantic python3-pydantic-settings \
        python3-pytest python3-sqlalchemy python3-uvicorn libgtest-dev libspdlog-dev libfmt-dev
    do
        if dpkg-query -W -f='${Status}' "$pkg" >/dev/null 2>&1; then
            printf "${ANSI_RESET}${ANSI_GREEN}Package %s already installed; skipping reinstall.${ANSI_RESET}\n" "$pkg"
            continue
        fi

        printf "${ANSI_RESET}${ANSI_GREEN}Installing package %s...${ANSI_RESET}\n" "$pkg"
        if sudo apt-get install -y "$pkg"; then
            printf "${ANSI_RESET}${ANSI_GREEN}Package %s installed successfully.${ANSI_RESET}\n" "$pkg"
        else
            printf "${ANSI_RESET}${ANSI_RED}Package %s is not installed via apt; skipping (install manually if required).${ANSI_RESET}\n" "$pkg"
        fi
    done
}

ensure_vendoring() {
    make -C "$SCRIPT_DIR/libdmotservices/js" vendor
    make -C "$SCRIPT_DIR/libdmotservices/python" vendor
}

##
## Git operations
##

run_git_command() {
    local target_dir=$1
    shift

    local display_path
    if [ "$target_dir" = "$SCRIPT_DIR" ]; then
        display_path="$SCRIPT_DIR"
    else
        display_path=${target_dir#"$SCRIPT_DIR"/}
    fi

    echo "== $display_path"
    if ! pushd "$target_dir" >/dev/null 2>&1; then
        printf "${ANSI_RESET}${ANSI_RED}Unable to enter %s; skipping git command.${ANSI_RESET}\n" "$display_path" >&2
        return 1
    fi

    local status=0
    if ! git "$@"; then
        status=$?
        printf "${ANSI_RESET}${ANSI_RED}git %s failed in %s (exit %s).${ANSI_RESET}\n" "$*" "$display_path" "$status" >&2
    fi

    popd >/dev/null || true
    return $status
}

git_op() {
    local had_error=0

    if ! run_git_command "$SCRIPT_DIR" "$@"; then
        had_error=1
    fi

    for prj in "${SUB_PROJECTS[@]}"; do
        if ! run_git_command "$prj" "$@"; then
            had_error=1
        fi
    done

    if [ "$had_error" -eq 1 ]; then
        printf "${ANSI_RESET}${ANSI_RED}One or more git commands failed.${ANSI_RESET}\n" >&2
    fi

    return 0
}

##
## Arguments parsing
##

if [ -z "$1" ]; then
    usage
fi

iface="wlan0"
mock=0
install_deps=0
install_prj=0
build=0
do_git_diff=0
do_git_log=0
do_git_push=0
do_git_status=0
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
        --git-diff)
            do_git_diff=1
            ;;
        --git-log)
            do_git_log=1
            ;;
        --git-push)
            do_git_push=1
            ;;
        --git-status)
            do_git_status=1
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

##
## Execution
##

JAVA_HOME=${JAVA_HOME:-/usr/lib/jvm/java-17-openjdk-amd64}
if [ -d "$JAVA_HOME" ]; then
    export JAVA_HOME
else
    echo "Warning: JAVA_HOME ($JAVA_HOME) not found; skipping export." >&2
fi
echo "JAVA_HOME=$JAVA_HOME"
echo "PYTHONPATH=$PYTHONPATH"

## git operations
[ ! "$do_git_diff" -eq 1 ] || git_op diff
[ ! "$do_git_log" -eq 1 ] || git_op log
[ ! "$do_git_push" -eq 1 ] || git_op push
[ ! "$do_git_status" -eq 1 ] || git_op status -sb

## build, deploy, and run operations

[ ! "$install_deps" -eq 1 ] || (ensure_dependencies && ensure_vendoring)
[ ! "$build" -eq 1 ] || (ensure_vendoring && make -C "$SCRIPT_DIR" clean all docs)
[ ! "$install_prj" -eq 1 ] || make -C "$SCRIPT_DIR" install
[ ! "$run_tests" -eq 1 ] || (ensure_vendoring && make -C "$SCRIPT_DIR" test)
[ ! "$run_examples" -eq 1 ] || (ensure_vendoring && WFQ_IFACE="$iface" WFQ_MOCK="$mock" make -C "$SCRIPT_DIR" example-demo)
