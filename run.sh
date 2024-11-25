#!/bin/bash
set -e
g++ --std=c++17 -O2 src/$1.cc -o target/$1
target/$1
