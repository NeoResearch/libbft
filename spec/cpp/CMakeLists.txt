cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME libbft)
set(PROJECT_VERSION 0.0.1)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
add_definitions(-std=c++17)
add_definitions(-D_GLIBCXX_DEBUG)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Ofast -Wextra -Wno-long-long -Wno-unused-parameter")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

add_subdirectory(src)
add_subdirectory(tests)
include_directories(src)
