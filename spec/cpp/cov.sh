#!/usr/bin/env bash
lcov -d . -c -o coverage.info
lcov --remove coverage.info '*/cpp/tests/*' '/usr/*' --output-file coverage.info
debug before upload
lcov --list coverage.info
genhtml coverage.info --output-directory out
