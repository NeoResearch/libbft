#!/usr/bin/env bash
rm -rf ${1}/grpc/third_party/protobuf/*.log
rm -rf ${1}/grpc/third_party/protobuf/libtool

rm -rf ${1}/grpc/third_party/protobuf/third_party/googletest/autom4te.cache/requests
rm -rf ${1}/grpc/third_party/protobuf/third_party/googletest/*.log
rm -rf ${1}/grpc/third_party/protobuf/third_party/googletest/googlemock/autom4te.cache/requests
rm -rf ${1}/grpc/third_party/protobuf/third_party/googletest/googlemock/*.log
rm -rf ${1}/grpc/third_party/protobuf/third_party/googletest/googlemock/libtool
rm -rf ${1}/grpc/third_party/protobuf/third_party/googletest/googletest/autom4te.cache/requests
rm -rf ${1}/grpc/third_party/protobuf/third_party/googletest/googletest/*.log
rm -rf ${1}/grpc/third_party/protobuf/third_party/googletest/googletest/libtool
#rm -rf ${1}/grpc/third_party/protobuf/third_party/

rm -rf ${1}/grpc/libs/opt/*.a
rm -rf ${1}/grpc/libs/opt/libgrpc++_error_details.so.1.27.0-dev
rm -rf ${1}/grpc/libs/opt/libgrpcpp_channelz.so.1.27.0-dev
rm -rf ${1}/grpc/libs/opt/libgrpc++_reflection.so.1.27.0-dev
rm -rf ${1}/grpc/libs/opt/libgrpc++_error_details.so.1.27.0-dev
rm -rf ${1}/grpc/libs/opt/libgrpcpp_channelz.so.1.27.0-dev
rm -rf ${1}/grpc/libs/opt/libg
rm -rf ${1}/grpc/libs/opt/libgrpc_cronet.so.9.0.0
rm -rf ${1}/grpc/libs/opt/libgrpc++.so.1.27.0-dev
rm -rf ${1}/grpc/libs/opt/libgrpc.so.9.0.0
rm -rf ${1}/grpc/libs/opt/libgrpc++_unsecure.so.1.27.0-dev
rm -rf ${1}/grpc/libs/opt/libgrpc_unsecure.so.9.0.0

rm -rf ${1}/grpc/objs/opt/${1}/gens/src/proto/grpc/channelz/*.o
rm -rf ${1}/grpc/objs/opt/${1}/grpc/gens/src/proto/grpc/core/*.o
rm -rf ${1}/grpc/objs/opt/${1}/grpc/gens/src/proto/grpc/reflection/v1alpha/*.o
#rm -rf ${1}/grpc/objs/opt/home/travis/build/NeoResearch/libbft/grpc/gens/
