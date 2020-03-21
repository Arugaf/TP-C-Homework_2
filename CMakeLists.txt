cmake_minimum_required(VERSION 3.15)

# Project
set(project_name Triangle_matrix)
project(${project_name})

# CMake
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-ldl -pthread -Wall -Werror -Wpedantic")

# Includes
set(includes ${PROJECT_SOURCE_DIR}/include)
include_directories(${includes})

set(includes_time_test ${PROJECT_SOURCE_DIR}/time_tests/include)

# Sources
set(sources ${PROJECT_SOURCE_DIR}/src)
set(src_files_lib ${sources}/triangle_matrix.c)
set(src_files_static ${sources}/tm_sum_cons.c)
set(src_files_dynamic ${sources}/tm_sum_parallel.c)

set(sources_time_tests ${PROJECT_SOURCE_DIR}/time_tests/src)
set(src_files_test_common ${sources_time_tests}/time_test.c)
set(src_files_test_static ${sources_time_tests}/time_test_cons.c)
set(src_files_test_dynamic ${sources_time_tests}/time_test_parallel.c)

# Libraries
add_library(${project_name}_static STATIC ${src_files_lib} ${src_files_static})
add_library(${project_name}_dynamic SHARED ${src_files_lib} ${src_files_dynamic})

# Executables
add_executable(time_test_parallel ${src_files_test_common} ${src_files_test_dynamic})
target_link_libraries(time_test_parallel ${CMAKE_DL_LIBS})
target_include_directories(time_test_parallel PUBLIC ${PROJECT_SOURCE_DIR}/time_tests/include)

add_executable(time_test_consecutive ${src_files_test_common} ${src_files_test_static})
target_link_libraries(time_test_consecutive ${project_name}_static)
target_include_directories(time_test_consecutive PUBLIC ${PROJECT_SOURCE_DIR}/time_tests/include)