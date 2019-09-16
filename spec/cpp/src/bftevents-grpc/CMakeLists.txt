cmake_minimum_required(VERSION 3.12)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP_UNSECURE gRPC::grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

file(GLOB PROTOBUF_DEFINITION_FILES "*.proto")
set(PROTOBUF_INPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")
set(PROTOBUF_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")

foreach(file ${PROTOBUF_DEFINITION_FILES})
#    execute_process(
#        COMMAND "protoc" "--proto_path=${PROTOBUF_INPUT_DIRECTORY}" "--cpp_out=${PROTOBUF_OUTPUT_DIRECTORY}" ${file}
#        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#        RESULT_VARIABLE PROTOBUF_RESULT
#        OUTPUT_VARIABLE PROTOBUF_OUTPUT_VARIABLE
#    )
#
#    if ("${PROTOBUF_RESULT}" STREQUAL "0")
#        message(STATUS "Created Protobuf files for ${file}")
#    else()
#        message(SEND_ERROR "Problem Protobuf generating files for ${file} - ${PROTOBUF_OUTPUT_VARIABLE}")
#    endif()
#
#    set(GRPC_CPP_PLUGIN_PATH "/usr/local/bin/grpc_cpp_plugin")
#    execute_process(
#        COMMAND "protoc"
#            "--proto_path=${PROTOBUF_INPUT_DIRECTORY}"
#            "--grpc_out=${PROTOBUF_OUTPUT_DIRECTORY}"
#            "--plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_PATH}"
#            ${file}
#        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#        RESULT_VARIABLE PROTOBUF_RESULT
#        OUTPUT_VARIABLE PROTOBUF_OUTPUT_VARIABLE
#    )
#    if ("${PROTOBUF_RESULT}" STREQUAL "0")
#        message(STATUS "Created gRPC files for ${file}")
#    else()
#        message(SEND_ERROR "Problem gRPC generating files for ${file} - ${PROTOBUF_OUTPUT_VARIABLE}")
#    endif()

    message(STATUS "hi ${file}")
    add_custom_command(
        OUTPUT "bftevent.grpc.pb.h" "bftevent.grpc.pb.cc" "bftevent.pb.h" "bftevent.pb.cc"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${CMAKE_CURRENT_SOURCE_DIR}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${file}"
        DEPENDS "${file}"
    )
endforeach()

#file(GLOB PROTOBUF_MODELS_INCLUDES "*.pb.cc" "*.hpp")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_executable(bftclient bftclient.cc build/bftevent.grpc.pb.cc build/bftevent.pb.cc)
