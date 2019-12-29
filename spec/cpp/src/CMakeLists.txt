file(GLOB_RECURSE project_sources *)
list(FILTER project_sources EXCLUDE REGEX "main.*\\.cpp$")
list(FILTER project_sources INCLUDE REGEX "pp$")

set(lib_name "${PROJECT_NAME}_${PROJECT_VERSION}")
message(STATUS "${lib_name} - ${project_sources}")
add_library(${lib_name} ${project_sources})
#set_source_files_properties(${project_sources} PROPERTIES LANGUAGE CXX)
set_target_properties(${lib_name} PROPERTIES LINKER_LANGUAGE CXX)

add_executable(app_test main.cpp ${project_sources})

target_link_libraries(
    app_test
    ${lib_name}
    protobuf
    grpc++
    grpc++_reflection
    dl
    pthread
)

add_subdirectory(bftevents-grpc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/lib")
add_executable(
    app_RPCtest
    mainRPC.cpp
    bftevents-grpc/bftevent.pb.cc
    bftevents-grpc/bftevent.grpc.pb.cc
    ${project_sources}
)

target_link_libraries(
    app_RPCtest
    protobuf
    pthread
    grpc++
    grpc++_reflection
    dl
)

if (build_type STREQUAL "debug")
    target_link_libraries(app_test asan)
    target_link_libraries(app_RPCtest asan)
endif()
