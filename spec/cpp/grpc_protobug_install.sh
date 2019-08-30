#!/usr/bin/env bash

cd ../../
git submodule update --init --recursive   # get submodules for libbft (including grpc)
cd spec/cpp/grpc/third_party/protobuf     # install protobuf first
git submodule update --init --recursive   # get submodules (for protobuf)
./autogen.sh                              # generate installation files
./configure
make
make check                                # if this fails, something bad happened
sudo make install                         # this will install system-wide protobuf and protoc (compiler)
sudo ldconfig                             # refresh shared library cache
cd ../..                                  # back to grpc
make
sudo make install                         # install grpc (and also /usr/local/bin/grpc_cpp_plugin)

# Testing outputs
protoc --version #libprotoc 3.8.0
ls -la /usr/local/lib/libproto* #/usr/local/lib/libprotobuf.so -> libprotobuf.so.19.0.0
