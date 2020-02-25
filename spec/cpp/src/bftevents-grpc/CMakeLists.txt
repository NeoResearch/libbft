find_package(ProtobufHelper REQUIRED)

file(GLOB DATAMODEL_PROTOS bftevent.proto)

foreach(proto ${DATAMODEL_PROTOS})
    GENERATE_PROTO(${proto})
endforeach(proto)
