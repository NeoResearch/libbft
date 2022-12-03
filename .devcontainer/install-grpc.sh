#!/bin/bash
# from: https://grpc.io/docs/languages/cpp/quickstart/

# =================================================
# THIS IS NOT NECESSARY FOR BAZEL... ONLY FOR CMAKE
# WARNING: IT WILL TAKE SOME MINUTES TO BUILD!
# =================================================
#
echo "THIS WILL TAKE LONG TIME..."
# downloading
MY_INSTALL_DIR=/opt
cd $MY_INSTALL_DIR
git clone --recurse-submodules -b v1.50.1 --depth 1 --shallow-submodules https://github.com/grpc/grpc
# building
cd grpc
mkdir -p cmake/build
pushd cmake/build
/opt/bin/cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_CXX_STANDARD=17 \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
make -j 4
make install
popd