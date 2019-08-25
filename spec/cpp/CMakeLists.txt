cmake_minimum_required(VERSION 3.8)
project(libbft VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Ofast")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests/libgtest)

file(GLOB_RECURSE test_sources *.cpp **/*.cpp)
add_executable(tests ${test_sources} tests/timing/ClockTests.cpp)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_options(tests PRIVATE "-Wall")
