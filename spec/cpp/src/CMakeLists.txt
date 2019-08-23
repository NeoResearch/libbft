add_executable(app_test main.cpp)

set_target_properties(app_test PROPERTIES
    CXX_STANDARD 17
)

# should do in other way I guess
# should be: `pkg-config --libs protobuf grpc++` and also -lgrpc++_reflection 
# which is: -L/usr/local/lib -lprotobuf -lgrpc++ -lgrpc++_reflection 
# perhaps -ldl (I don't know why)
# and also bftevent.pb.o and bftevent.grpc.pb.o (how to add them?)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Ofast -L/usr/local/lib -lprotobuf -lgrpc++ -lgrpc++_reflection -ldl ../src/bftevents-grpc/bftevent.pb.o ")

add_executable(app_RPCtest mainRPC.cpp)

set_target_properties(app_RPCtest PROPERTIES
    CXX_STANDARD 17
)
