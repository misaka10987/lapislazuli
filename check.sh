#!/bin/bash
set -e

main() {
    clang++ -std=c++23 -Wall -fsyntax-only $1
    set +e
    g++ -std=c++17 -o /dev/null $1 >/dev/null 2>/dev/null
    local succ=$?
    set -e
    if [ $succ -ne 0 ]; then
        echo "failure"
        exit $succ
    fi
    echo "success"
}

main $@
