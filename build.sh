#!/bin/bash
set -e
mkdir -p ./target
gcc -fpreprocessed -dD -E ./src/lib.cc > ./target/lapislazuli.cc
vim -c ":%s/\(\"[^\"]*\)\@<!\s\{2,}/ /g|:%j|:%s/\(.\)\zs#\([^#]\)/\r\1#\2/g|:%s/namespace/\rnamespace/g|:wq" ./target/lapislazuli.cc
