cmake_minimum_required(VERSION 3.8)
project(libbft VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Ofast")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

add_subdirectory(src)
add_subdirectory(tests/libgtest)
