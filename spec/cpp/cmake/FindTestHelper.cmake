MACRO(ADD_TEST_ALL_FILES)
    get_filename_component(test_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    message(STATUS "Creating test ${test_name}")

    file(GLOB_RECURSE test_sources *)
    list(FILTER test_sources INCLUDE REGEX "cpp$")

    add_executable("${test_name}_test" ${test_sources})

    target_link_libraries(
            "${test_name}_test"
            gtest
            gtest_main
            ${CMAKE_THREAD_LIBS_INIT}
    )
    if (build_type STREQUAL "debug")
        target_link_libraries("${test_name}_test" asan)
    endif()
    target_link_libraries("${test_name}_test" ${lib_name})
    include_directories(${CMAKE_SOURCE_DIR}/src)

    message(STATUS "Adding test: ${test_name}_test")
    add_test(NAME "${test_name}_test_caller" COMMAND "${test_name}_test")

    target_compile_options("${test_name}_test" PRIVATE --coverage)
    target_link_libraries("${test_name}_test" --coverage)
ENDMACRO()
