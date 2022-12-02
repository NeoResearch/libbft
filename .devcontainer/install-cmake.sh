#!/bin/bash
# from: https://grpc.io/docs/languages/cpp/quickstart/

MY_INSTALL_DIR=/opt
cd $MY_INSTALL_DIR
wget -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v3.19.6/cmake-3.19.6-Linux-x86_64.sh
sh cmake-linux.sh -- --skip-license --prefix=$MY_INSTALL_DIR
rm cmake-linux.sh
