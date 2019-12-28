#!/usr/bin/env bash
if [[ -f "${1}grpc/" ]]; then
    echo "Installing gRPC"
    cd ${1}grpc/
    make
    sudo make install
else
    echo "gRPC is already installed"
fi
