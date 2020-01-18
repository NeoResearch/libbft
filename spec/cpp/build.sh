#!/usr/bin/env bash
cmake -Dtest=ON -DCOVERAGE=1 ..
make VERBOSE=1
GTEST_COLOR=1 ctest --extra-verbose
