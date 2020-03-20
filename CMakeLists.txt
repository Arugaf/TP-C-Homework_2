cmake_minimum_required(VERSION 3.15)

# Project
set(project_name Triangle_matrix)
project(${project_name})

# CMake
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-pthread -Wall -Werror -Wpedantic")

# Includes
set(includes ${PROJECT_SOURCE_DIR}/include)
include_directories(${includes})

# Sources
set(sources ${PROJECT_SOURCE_DIR}/src)
set(src_files_lib ${sources}/triangle_matrix.c)
set(src_files_static ${sources}/tm_sum_cons.c)
set(src_files_dynamic ${sources}/tm_sum_parallel.c)

# Libraries
add_library(${project_name}_static STATIC ${src_files_lib} ${src_files_static})
add_library(${project_name}_dynamic STATIC ${src_files_lib} ${src_files_dynamic})

# Executables
add_executable(library_testing ${sources}/main.c)
target_link_libraries(library_testing ${project_name}_dynamic)