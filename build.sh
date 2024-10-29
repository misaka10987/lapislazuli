#!/bin/bash
set -e

build() {
    mkdir -p ./target
    gcc -fpreprocessed -dD -E ./src/lib.cc >./target/lapislazuli
    vim -c ":%s/^# .*//g|:%s/\(\"[^\"]*\)\@<!\s\{2,}/ /g|:%j|:%s/\(.\)\zs#\([^#]\)/\r\1#\2/g|:%s/namespace/\rnamespace/g|:%s/ #/#/|:wq" ./target/lapislazuli
}

subs() {
    sed -e '/#define lapislazuli/ {' -e 'r ./target/lapislazuli' -e 'd' -e '}' -i $1
}

main() {
    if [ $# -eq 0 ]; then
        build
    else
        subs $1
    fi
}

main $@
