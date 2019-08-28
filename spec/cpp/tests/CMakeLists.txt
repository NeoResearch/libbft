MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child} AND EXISTS ${curdir}/${child}/CMakeLists.txt)
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE test_sources *)
list(FILTER test_sources INCLUDE REGEX "cpp$")

add_executable(test ${test_sources})
target_link_libraries(test gtest gtest_main)
target_link_libraries(test ${lib_name})

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

FOREACH(subdir ${SUBDIRS})
    ADD_SUBDIRECTORY(${subdir})
ENDFOREACH()
