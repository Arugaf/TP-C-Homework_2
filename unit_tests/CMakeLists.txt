# Project
set(tests_name triangle_matrix_test)

# CMake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "-ldl -pthread")

# Includes
set(sources_tests ${PROJECT_SOURCE_DIR}/unit_tests)

# Sources
set(src_files ${sources_tests}/main.cpp)
set(src_files_static ${sources_tests}/cons_test.cpp)
set(src_files_dynamic ${sources_tests}/multithread_test.cpp)

# Executables
add_executable(${tests_name}_static ${src_files} ${src_files_static})
add_executable(${tests_name}_dynamic ${src_files} ${src_files_dynamic})

target_link_libraries(${tests_name}_static gtest_main gtest ${project_name}_static)
target_link_libraries(${tests_name}_dynamic gtest_main gtest ${CMAKE_DL_LIBS})

# Tests
add_test(NAME ${tests_name}_static COMMAND ${tests_name}_static)
add_test(NAME ${tests_name}_dynamic COMMAND ${tests_name}_dynamic)