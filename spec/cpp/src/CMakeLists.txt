file(GLOB_RECURSE project_sources *)
list(FILTER project_sources EXCLUDE REGEX "main.*\\.cpp$")
list(FILTER project_sources INCLUDE REGEX "pp$")

set(lib_name "${PROJECT_NAME}_${PROJECT_VERSION}")
#message(STATUS "${lib_name} - ${project_sources}")
add_library(${lib_name} ${project_sources})
set_target_properties(${lib_name} PROPERTIES LINKER_LANGUAGE CXX)

add_executable(app_test main.cpp)
target_link_libraries(app_test ${lib_name})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Ofast -lpthread -L/usr/local/lib -lprotobuf -lgrpc++ -lgrpc++_reflection -ldl ${PROJECT_SOURCE_DIR}/src/bftevents-grpc/bftevent.pb.o ${PROJECT_SOURCE_DIR}/src/bftevents-grpc/bftevent.grpc.pb.o ")
add_executable(app_RPCtest mainRPC.cpp)

# TODO: set target for app_RPCtest
#set_target_properties(app_RPCtest PROPERTIES
#    CXX_STANDARD 17
#)
