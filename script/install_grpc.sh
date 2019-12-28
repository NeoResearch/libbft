#!/usr/bin/env bash
echo "Checking folder ${1}grpc/"
if test -f "${1}grpc/"; then
    echo "Installing gRPC"
    cd ${1}grpc
    sudo make install
else
    echo "Building and installing gRPC"
    cd ${1}grpc
    make
    sudo make install
fi
