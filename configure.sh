#!/usr/bin/env bash
projects_folder=spec

git submodule update --remote --init --recursive
git pull --recurse-submodules
#sudo apt-get install valgrind

rm -rf ~/go/src/github.com/NeoResearch
mkdir -p ~/go/src/github.com/NeoResearch
ln -s $(pwd)/${projects_folder}/go ~/go/src/github.com/NeoResearch/libbft
