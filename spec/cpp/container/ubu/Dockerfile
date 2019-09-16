FROM ubuntu:18.10

RUN apt update
RUN apt -y install wget git cmake
RUN apt -y install build-essential autoconf libtool pkg-config
RUN apt -y install libgflags-dev libgtest-dev
RUN apt -y install clang-7 libc++-dev libssl-dev

# Install golang
RUN cd /tmp
RUN wget https://dl.google.com/go/go1.13.linux-amd64.tar.gz
RUN tar -xzvf go1.13.linux-amd64.tar.gz
RUN mv go /usr/local
RUN export GOPATH=$HOME/
RUN export PATH=$PATH:/usr/local/go/bin:$GOPATH/bin
RUN rm go1.13.linux-amd64.tar.gz

# Install gRPC
RUN cd /tmp
RUN git clone https://github.com/grpc/grpc.git
RUN cd grpc
RUN git submodule update --init --recursive
RUN cd third_party/boringssl
RUN git checkout master
RUN cd /tmp/grpc
RUN mkdir build
RUN cd build
RUN cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DgRPC_PROTOBUF_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package -DgRPC_CARES_PROVIDER=package -DgRPC_SSL_PROVIDER=package -DCMAKE_BUILD_TYPE=Release ..
RUN make
RUN make install
