cmake_minimum_required(VERSION 3.15)

set(project_name Triangle_matrix)
project(${project_name})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic")

set(includes ${PROJECT_SOURCE_DIR}/include)
include_directories(${includes})

set(sources ${PROJECT_SOURCE_DIR}/src)
set(src_files ${sources}/tm_sum_cons.c
              ${sources}/triangle_matrix.c)

add_library(${project_name}_static STATIC ${src_files})
add_library(${project_name}_dynamic MODULE ${src_files})

add_executable(library_testing ${sources}/main.c)
target_link_libraries(library_testing ${project_name}_static)