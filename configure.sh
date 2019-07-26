#!/usr/bin/env bash
projects_folder=spec

git submodule update --remote --init --recursive
git pull --recurse-submodules
#sudo apt-get install valgrind

mkdir -p ~/go/src/github.com
ln -s $(pwd)/${projects_folder}/go/ ~/go/src/github.com/libbft
