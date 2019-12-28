#!/usr/bin/env bash
echo "Checking folder ${1}grpc/"
if test -f "${1}grpc/"; then
    echo "Installing Protobuf"
    cd ${1}grpc/third_party/protobuf
    git submodule update --init --recursive
    sh autogen.sh
    sh configure --prefix=/usr
    make
    sudo make install
    sudo ldconfig
else
    echo "Protobuf is already installed"
fi
