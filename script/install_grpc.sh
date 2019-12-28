#!/usr/bin/env bash
echo "Installing gRPC"
cd $1/grpc
make
sudo make install
