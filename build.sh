#!/bin/bash
set -o errexit
cmake -B build_linux
cmake --build build_linux --config Release
ctest --verbose -C Release --test-dir build_linux
