include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

get_filename_component(test_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB_RECURSE test_sources *)
list(FILTER test_sources INCLUDE REGEX "cpp$")
add_executable("${test_name}_test" ${test_sources})

target_link_libraries("${test_name}_test" gtest gtest_main)
target_link_libraries("${test_name}_test" ${lib_name})
include_directories(${CMAKE_SOURCE_DIR}/src)
