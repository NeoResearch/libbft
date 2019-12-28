#!/usr/bin/env bash
echo "Checking folder ${1}grpc/"
if test -f "${1}grpc/"; then
    echo "Installing gRPC"
    cd ${1}grpc/
    make
    sudo make install
else
    echo "gRPC is already installed"
fi
