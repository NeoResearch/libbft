add_executable(app_test main.cpp)

set_target_properties(app_test PROPERTIES
    CXX_STANDARD 17
)

add_executable(app_RPCtest mainRPC.cpp)

set_target_properties(app_RPCtest PROPERTIES
    CXX_STANDARD 17
)
