cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME libbft)
set(PROJECT_VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

#if (EXISTS /usr/bin/gcc-11)
#    if (EXISTS /usr/bin/g++-11)
#        set(CMAKE_C_COMPILER /usr/bin/gcc-11)
#        set(CMAKE_CXX_COMPILER /usr/bin/g++-11)
#    endif()
#endif()


set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Definitions REQUIRED)

ADD_MY_DEFINITIONS()

#set(CMAKE_CXX_SOURCE_FILE_EXTENSIONS "${CMAKE_CXX_SOURCE_FILE_EXTENSIONS};hpp;HPP")
#message(STATUS "Ignoring extensios ${CMAKE_CXX_IGNORE_EXTENSIONS}")
#list(REMOVE_ITEM CMAKE_CXX_IGNORE_EXTENSIONS HPP hpp)
#message(STATUS "Ignoring extensios ${CMAKE_CXX_IGNORE_EXTENSIONS}")

#set(PROTOBUF_GENERATED_PATH src CACHE STRING proto_gen)
#
#
#set(Protobuf_INCLUDE_DIRS /opt/include)
#
#
# IMPORTANT: ln -s /opt/bin/grpc_cpp_plugin /usr/local/bin/grpc_cpp_plugin
set(GRPC_PLUGIN_CPP /usr/local/bin/grpc_cpp_plugin CACHE STRING grpc_bin)

option(BUILD_TESTS "Build test programs" ON)
message(STATUS "BUILD_TESTS = ${BUILD_TESTS}")

set(CMAKE_CXX_CPPCHECK "cppcheck")

include_directories(src)
add_subdirectory(src)
if (BUILD_TESTS)
    message(STATUS "Building the tests")
    enable_testing()
    SET(COVERAGE OFF CACHE BOOL "Coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    add_definitions(--coverage)
    add_definitions(-fprofile-arcs)
    add_definitions(-ftest-coverage)
    #find_package(GTest REQUIRED)
    #include_directories(${GTEST_INCLUDE_DIRS})
    add_subdirectory(tests)
else()
    message(STATUS "Ignoring the tests")
endif()
