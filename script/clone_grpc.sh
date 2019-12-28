#!/usr/bin/env bash
if [[ -f "${1}grpc/" ]]; then
    echo "Cloning gRPC"
    cd ${1}
    git clone https://github.com/grpc/grpc
    cd grpc
    echo "Cloning Protobuf"
    git submodule update --init --recursive
else
    echo "gRPC is already installed"
fi
