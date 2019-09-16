cmake_minimum_required(VERSION 3.12)
project(libbft VERSION 0.1)

add_definitions(-O3)
#add_definitions(-std=c++17)
#add_definitions(-fconcepts )
#add_definitions(-D_GLIBCXX_DEBUG)

file(GLOB_RECURSE project_sources *)
list(FILTER project_sources EXCLUDE REGEX "main.*\\.cpp$")
list(FILTER project_sources INCLUDE REGEX "pp$")

set(lib_name "${PROJECT_NAME}_${PROJECT_VERSION}")
#message(STATUS "${lib_name} - ${project_sources}")
add_library(${lib_name} ${project_sources})
set_target_properties(${lib_name} PROPERTIES LINKER_LANGUAGE CXX)

add_executable(app_test main.cpp)
target_link_libraries(app_test ${lib_name})

target_link_libraries(app_test protobuf)
target_link_libraries(app_test grpc++)
target_link_libraries(app_test grpc++_reflection)
target_link_libraries(app_test dl)
target_link_libraries(app_test pthread)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Ofast -lpthread -L/usr/local/lib -lprotobuf -lgrpc++ -lgrpc++_reflection -ldl ${PROJECT_SOURCE_DIR}/bftevents-grpc/bftevent.pb.o ${PROJECT_SOURCE_DIR}/bftevents-grpc/bftevent.grpc.pb.o ")
add_executable(app_RPCtest mainRPC.cpp)

target_link_libraries(app_RPCtest protobuf)
target_link_libraries(app_RPCtest grpc++)
target_link_libraries(app_RPCtest grpc++_reflection)
target_link_libraries(app_RPCtest dl)
target_link_libraries(app_RPCtest pthread)

#add_subdirectory(bftevents-grpc)

# TODO: set target for app_RPCtest
#set_target_properties(app_RPCtest PROPERTIES
#    CXX_STANDARD 17
#)
