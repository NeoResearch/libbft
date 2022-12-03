find_package(ProtobufHelper REQUIRED)

file(GLOB DATAMODEL_PROTOS bftevent.proto)

foreach(proto ${DATAMODEL_PROTOS})
    GENERATE_PROTO(${proto})
endforeach(proto)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/lib -L/opt/lib")
add_executable(
    bftclient
    mainBftclient.cc
    bftevent.pb.cc
    bftevent.grpc.pb.cc
)

target_link_libraries_proto(bftclient)

add_executable(
    bftserver
    mainBftserver.cc
    bftevent.pb.cc
    bftevent.grpc.pb.cc
)

target_link_libraries_proto(bftserver)
