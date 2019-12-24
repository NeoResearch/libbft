cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME libbft)
set(PROJECT_VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

# https://developers.redhat.com/blog/2018/03/21/compiler-and-linker-flags-gcc/
add_definitions(-std=c++17)
add_definitions(-fconcepts)
add_definitions(-fpermissive)
add_definitions(-Wall)
add_definitions(-fasynchronous-unwind-tables)
add_definitions(-pipe)
add_definitions(-Werror=format-security)
add_definitions(-Werror=implicit-function-declaration)
# https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html
add_definitions(-Wpedantic)
add_definitions(-W)
add_definitions(-Wconversion)
add_definitions(-Wcast-align)
add_definitions(-Wunused)
add_definitions(-Wshadow)
add_definitions(-Wold-style-cast)
add_definitions(-Wpointer-arith)
add_definitions(-Wcast-qual)
add_definitions(-Wno-missing-braces)

add_definitions(-Ofast)
add_definitions(-Wall)
add_definitions(-Wextra)
add_definitions(-Wno-long-long)
add_definitions(-Wno-unused-parameter)
add_definitions(-Wno-switch)
add_definitions(-Wno-parentheses)
add_definitions(-Wunused-function)

#add_definitions(-Werror)
add_definitions(-pedantic-errors)
#add_definitions(-fno-rtti)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-prototypes")

string(TOLOWER "${CMAKE_BUILD_TYPE}" build_type)
if (build_type STREQUAL "debug")
    message(STATUS "--- Debug build ---")
    add_definitions(-g)
    add_definitions(-grecord-gcc-switches)
#    add_definitions(-D_GLIBCXX_DEBUG) # Not sure why it's breaking the mainRPC.cpp build
#    add_definitions(-D_GLIBCXX_DEBUG_PEDANTIC) # Not sure why it's breaking the mainRPC.cpp build
    add_definitions(-D_FORTIFY_SOURCE=2)
    add_definitions(-D_GLIBCXX_ASSERTIONS)
    add_definitions(-ggdb)
    add_definitions(-O0)
    add_definitions(-fstack-protector-all)
else ()
    message(STATUS "--- Release build ---")
    add_definitions(-O3)
    add_definitions(-finline-functions)
    add_definitions(-funroll-loops)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -frecursive")
endif ()

#set(PROTOBUF_GENERATED_PATH src CACHE STRING proto_gen)
set(GRPC_PLUGIN_CPP /usr/local/bin/grpc_cpp_plugin CACHE STRING grpc_bin)

message(STATUS "BUILD_TESTS = ${BUILD_TESTS}")

add_subdirectory(src)
#add_subdirectory(tests)
option(BUILD_TESTS "Build test programs" ON)
include_directories(src)
