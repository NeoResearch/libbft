MACRO(ADD_TEST_ALL_FILES asan)
    get_filename_component(test_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    if (${asan} STREQUAL "ON")
        message(STATUS "Adding address sanitizer to ${test_name}")
        ADD_TEST_DEFINITIONS()
    else ()
        message(STATUS "No address sanitizer to ${test_name}")
    endif ()
    message(STATUS "Creating test ${test_name}")

    file(GLOB_RECURSE test_sources *)
    list(FILTER test_sources INCLUDE REGEX "cpp$")

    set(lib_name "${PROJECT_NAME}_${PROJECT_VERSION}")

    add_executable(
        "${test_name}_test"
        ${test_sources}
    )

    target_link_libraries(
        "${test_name}_test"
        ${lib_name}
        gtest
        gtest_main
        ${CMAKE_THREAD_LIBS_INIT}
    )

    if (${asan} STREQUAL "ON")
        target_link_libraries("${test_name}_test" asan)
    endif ()

    target_link_libraries("${test_name}_test" ${lib_name})
    include_directories(${CMAKE_SOURCE_DIR}/src)

    message(STATUS "Adding test: ${test_name}_test")
    add_test(NAME "${test_name}_test_caller" COMMAND "${test_name}_test")

    target_compile_options("${test_name}_test" PRIVATE --coverage)
    target_link_libraries("${test_name}_test" --coverage)
ENDMACRO()
