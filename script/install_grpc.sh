#!/usr/bin/env bash
echo "Checking folder ${1}grpc/"
if test -f "${1}grpc/"; then
    echo "Installing gRPC"
    ls -lha
    cd ${1}
    ls -lha
    cd grpc
    ls -lha
    sudo make install
else
    echo "Building and installing gRPC"
    ls -lha
    cd ${1}
    ls -lha
    cd grpc
    ls -lha
    make
    sudo make install
fi
