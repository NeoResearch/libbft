find_package(ProtobufHelper REQUIRED)

file(GLOB DATAMODEL_PROTOS ${CMAKE_CURRENT_SOURCE_DIR}/bftevent.proto)

foreach(proto ${DATAMODEL_PROTOS})
    GENERATE_PROTO(${proto})
endforeach(proto)
