add_subdirectory(libgtest)

find_package(Helper REQUIRED)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

FOREACH (subdir ${SUBDIRS})
    if (subdir STREQUAL "libgtest")
        message(STATUS "This is the ${subdir}")
    else ()
        message(STATUS "Adding ${subdir}")
        add_subdirectory(${subdir})
    endif ()
ENDFOREACH ()
