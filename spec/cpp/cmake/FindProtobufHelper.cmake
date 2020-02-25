#================================================================#
#  Protobuf code generation
#================================================================#
find_package(Protobuf REQUIRED)

#include_directories(protos)

#file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} PROTOMODEL_PATH)
#file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${PROTOBUF_GENERATED_PATH} PROTOBINDING_PATH)

MACRO(GENERATE_PROTO proto)
    file(TO_NATIVE_PATH ${proto} proto_native)
    message(STATUS "Generating Protobuf ${proto}")
    execute_process(
        COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
        #        --proto_path=${PROTOMODEL_PATH}
        --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
        #        --cpp_out=${PROTOBINDING_PATH} ${proto_native}
        --cpp_out=${CMAKE_CURRENT_SOURCE_DIR} ${proto_native}
        RESULT_VARIABLE rv
    )

    execute_process(
        COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
        #        --proto_path=${PROTOMODEL_PATH}
        --proto_path=${CMAKE_CURRENT_SOURCE_DIR}
        #        --grpc_out=${PROTOBINDING_PATH}
        --grpc_out=${CMAKE_CURRENT_SOURCE_DIR}
        --plugin=protoc-gen-grpc=${GRPC_PLUGIN_CPP} ${proto_native}
        RESULT_VARIABLE rv
    )
ENDMACRO()

MACRO(target_link_libraries_proto name)
    target_link_libraries(
        ${name}
        protobuf
        pthread
        grpc++
        grpc++_reflection
        dl
    )
ENDMACRO()
