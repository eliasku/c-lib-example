#!/usr/bin/env bash

set -e
set -x

cmake -S . -B build/debug \
-G Ninja \
-DCMAKE_C_COMPILER_LAUNCHER=ccache \
-DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
-DCMAKE_BUILD_TYPE=Profiling \
-DCODE_COVERAGE=ON \
-DUNIT_TESTING=ON

ninja coverage -C build/debug
