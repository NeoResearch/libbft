find_package(ProtobufHelper REQUIRED)

file(GLOB DATAMODEL_PROTOS bftp2p.proto)

foreach(proto ${DATAMODEL_PROTOS})
    GENERATE_PROTO(${proto})
endforeach(proto)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/lib -L/opt/lib")
add_executable(
    p2p
    mainP2P.cpp
    bftp2p.pb.cc
    bftp2p.grpc.pb.cc
    P2PServer.cpp
    ../utils/CLI.cpp
)

target_link_libraries_proto(p2p)

set(lib_name "${PROJECT_NAME}_${PROJECT_VERSION}")
message(STATUS "p2p - ${lib_name}")

# TODO Check why it does not link the CLI.cpp
target_link_libraries(
    ${name}
    ${lib_name}
)
