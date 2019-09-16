FROM grpc/cxx

RUN apt update
RUN apt -y install wget

# Installing directly from apt-get will get the version 3.7 but the project requires 3.13
RUN cd /tmp
RUN wget https://cmake.org/files/v3.13/cmake-3.13.5.tar.gz
RUN tar -xzvf cmake-3.13.5.tar.gz
RUN cd cmake-3.13.5

RUN ./bootstrap
RUN make -j4
RUN make install
RUN cp bin/* /usr/bin
RUN cd ..
RUN rm -rf cmake-3.13.5
RUN ldconfig

RUN cd /tmp
RUN git clone https://github.com/protocolbuffers/protobuf.git
RUN git submodule update --init --recursive
RUN ./autogen.sh
RUN ./configure --prefix=/usr
RUN make -j4
RUN make check
RUN make install
RUN ldconfig


RUN cd /tmp
RUN git clone https://github.com/grpc/grpc.git
RUN git submodule update --init --recursive
RUN mkdir grpc/build
RUN cd grpc/build
