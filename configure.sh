#!/usr/bin/env bash
git submodule update --remote --init --recursive
git pull --recurse-submodules
sudo apt-get install valgrind
