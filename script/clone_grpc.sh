#!/usr/bin/env bash
echo "Cloning gRPC"
cd $1
git clone https://github.com/grpc/grpc
cd grpc
echo "Cloning Protobuf"
git submodule update --init --recursive
