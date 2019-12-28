#!/usr/bin/env bash
echo "Checking folder ${1}grpc/"
if test -f "${1}grpc/"; then
    echo "gRPC folder is already here"
else
    echo "Cloning gRPC"
    cd ${1}
    git clone https://github.com/grpc/grpc
    cd grpc
    echo "Cloning Protobuf"
    git submodule update --init --recursive
fi
