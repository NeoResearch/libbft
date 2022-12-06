FROM mcr.microsoft.com/devcontainers/cpp:0-ubuntu-22.04

# general building stuff
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends build-essential autoconf libtool pkg-config

# install graphviz
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends graphviz 

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>

#RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends nodejs npm 

RUN curl -sL https://deb.nodesource.com/setup_18.x | sudo -E bash -

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends nodejs 

# install bazel
RUN npm install -g @bazel/bazelisk

# install vcpkg: https://vcpkg.io/en/getting-started.html
RUN (cd /opt && git clone https://github.com/Microsoft/vcpkg.git)
RUN /opt/vcpkg/bootstrap-vcpkg.sh

# install pip and cpplint
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends python3-pip
# install cpplint into /usr/local/bin/cpplint
RUN pip install cpplint 

# install protobuf
#RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends protobuf-compiler libprotobuf-dev
#RUN /opt/vcpkg/vcpkg install protobuf

# install grpc
#RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends libgrpc++-dev

# remove cmake
RUN apt-get update && apt-get -y remove cmake

# install cmake
COPY ./install-cmake.sh /tmp/
RUN chmod +x /tmp/install-cmake.sh && /tmp/install-cmake.sh && rm -f /tmp/install-cmake.sh

# build grpc
COPY ./install-grpc.sh /tmp/
RUN chmod +x /tmp/install-grpc.sh && /tmp/install-grpc.sh && rm -f /tmp/install-grpc.sh

# make shortcut to grpc_cpp_plugin
RUN ln -s /opt/bin/grpc_cpp_plugin /usr/local/bin/grpc_cpp_plugin

# MOVE UP:
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends clang-tidy clang-format clangd
