project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

file(GLOB_RECURSE project_sources *)
list(REMOVE_ITEM project_sources ${PROJECT_SOURCE_DIR}/src/main.cpp)
list(FILTER project_sources INCLUDE REGEX "pp$")

set(lib_name "${PROJECT_NAME}_${PROJECT_VERSION}")
#message(STATUS "${lib_name} - ${project_sources}")
add_library(${lib_name} ${project_sources})

add_executable(app_test main.cpp)
target_link_libraries(app_test ${lib_name})
