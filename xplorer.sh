#!/bin/bash

BUILD_DIR="out"
APP_TITLE="xplorer"

cmd_exists() { command -v "$1" &>/dev/null; }

path_exists() { [[ -e "$1" ]]; }

file_exists() { [[ -f "$1" ]]; }

dir_exists() { [[ -d "$1" ]]; }

is_empty() { [[ -z "${1:-}" ]]; }

starts_with() { [[ "$1" == "$2"* ]]; }

to_lower() { echo "$1" | tr '[:upper:]' '[:lower:]'; }

trim() { echo "$1" | xargs; }

log() { echo "$@"; }

log_err() { echo "$@" >&2; }

run_cmd() {
    log "running: $*"
    if ! "$@"; then
        log_err "failed: $*"
        return 1
    fi
}

run_cmd_silent() {
    if ! "$@" &>/dev/null; then
        log_err "failed: $*"
        return 1
    fi
}

remove_dir() { dir_exists "$1" && rm -rf "$1"; }

remove_file() { file_exists "$1" && rm -f "$1"; }

is_windows() {
    [[ "${OSTYPE:-}" == "msys" ]] || \
    [[ "${OSTYPE:-}" == "cygwin" ]] || \
    [[ "${OSTYPE:-}" == "win32" ]]
}

clear_terminal() {
    if is_windows; then
        cmd clear 2>/dev/null || clear
    else
        clear
    fi
}

read_file_lines() {
    local -n _lines="$1"
    mapfile -t _lines < "$2"
}

for_each_line() {
    local file="$1"
    local callback="$2"
    local line
    while IFS= read -r line || [[ -n "$line" ]]; do
        $callback "$line"
    done < "$file"
}

process_spec_line() {
    local line="$1"
    local pkg
    pkg=$(trim "$line")
    if ! is_empty "$pkg" && ! starts_with "$pkg" "#"; then
        run_cmd xmake require "$pkg"
    fi
}

prompt_read() {
    local -n _result="$1"
    local prompt="$2"
    read -rp "$prompt" _result
}

default_or() {
    echo "${1:-$2}"
}

init_xmake() {
    if cmd_exists xmake; then
        log "xmake already installed"
        return 0
    fi
    log "installing xmake..."
    run_cmd bash -c "curl -fsSL https://xmake.io/shget.text | bash"
}

build() {
    local mode
    mode=$(default_or "$1" "release")
    log "building in $mode mode..."
    run_cmd xmake f -o "$BUILD_DIR" -m "$mode"
    run_cmd xmake
    remove_dir "build"
}

clean() {
    log "cleaning..."
    run_cmd xmake clean -a
    remove_dir "$BUILD_DIR"
    remove_dir "build"
    remove_dir ".xmake"
}

run_app() {
    run_cmd xmake run "$APP_TITLE"
}

debug() {
    build "debug"
    run_cmd xmake run -d "$APP_TITLE"
}

install_specs() {
    if ! file_exists "spec.md"; then
        log "spec.md missing"
        return 1
    fi
    log "installing specs..."
    for_each_line "spec.md" process_spec_line
}

show_prompt() {
    echo -n "[b]uild [c]lean [r]un [d]ebug [s]pecs [k]lear [i]nit e[x]it > "
}

resolve_command() {
    local char="$1"
    case "$(to_lower "$char")" in
        b) echo "build" ;;
        c) echo "clean" ;;
        r) echo "run" ;;
        d) echo "debug" ;;
        s) echo "specs" ;;
        k) echo "clear" ;;
        i) echo "init" ;;
        x) echo "exit" ;;
        *) echo "unknown" ;;
    esac
}

dispatch() {
    local cmd="$1"
    case "$cmd" in
        build) build ;;
        clean) clean ;;
        run)   run_app ;;
        debug) debug ;;
        specs) install_specs ;;
        clear) clear_terminal ;;
        init)  init_xmake ;;
        exit)  return 1 ;;
        *)     log "what!?" ;;
    esac
    return 0
}

handle_input() {
    local raw="$1"
    local cmd
    raw=$(trim "$raw")
    is_empty "$raw" && return 0
    cmd=$(resolve_command "$raw")
    dispatch "$cmd"
}

main_loop() {
    local input
    while true; do
        show_prompt
        prompt_read input ""
        handle_input "$input" || break
    done
}

do_main() {
    main_loop
    log "bye!"
}

if [ -t 0 ]; then
    do_main
else
    title="$BIN"
    xfce4-terminal\
        --title="$title"\
        -e "bash -c '$0 $@; exec bash'"
fi