//
// Created by arugaf on 21.03.2020.
//

#include "time_test.h"
#include "triangle_matrix.h"

#include <dlfcn.h>
#include <pthread.h>

#define FAILED_TO_OPEN_LIBRARY -1
#define FAILED_TO_JOIN_FUNC -2
#define EXECUTION_ERROR -3

#define DEFAULT_NUMBER_OF_ITERATIONS 10000
#define DEFAULT_NUMBER_OF_THREADS 8

int main(int argc, char** argv) {
    size_t number_of_iterations = DEFAULT_NUMBER_OF_ITERATIONS;
    size_t matrix_size = DEFAULT_MATRIX_SIZE;
    size_t number_of_threads = DEFAULT_NUMBER_OF_THREADS;

    if (argc > 1) {
        number_of_iterations = strtol(argv[1], NULL, 10);
        if (number_of_iterations < 1) {
            number_of_iterations = DEFAULT_NUMBER_OF_ITERATIONS;
        }
    }

    if (argc > 2) {
        matrix_size = strtol(argv[2], NULL, 10);
        if (matrix_size < 1) {
            matrix_size = DEFAULT_MATRIX_SIZE;
        }
    }

    if (argc > 3) {
        number_of_threads = strtol(argv[3], NULL, 10);
        if (number_of_threads < 1) {
            number_of_threads = DEFAULT_NUMBER_OF_THREADS;
        }
    }

    // =============================================================================
    //                                  DLOpen
    // =============================================================================

    void* library = dlopen("./libTriangle_matrix_dynamic.so", RTLD_LAZY);

    if (!library) {
        return FAILED_TO_OPEN_LIBRARY;
    }

    void* (*create_matrix)(const size_t size) = NULL;
    void* (*free_matrix)(triangle_matrix* matrix) = NULL;
    void* (*fill_matrix_consecutive)(triangle_matrix* matrix) = NULL;
    void* (*calculate_diagonal_sum)(triangle_matrix* matrix, unsigned long int* result, ...) = NULL;

    *(void**)(&create_matrix) = dlsym(library, "create_matrix");
    if (!create_matrix) {
        dlclose(library);
        return FAILED_TO_JOIN_FUNC;
    }

    *(void**)(&free_matrix) = dlsym(library, "free_matrix");
    if (!free_matrix) {
        dlclose(library);
        return FAILED_TO_JOIN_FUNC;
    }

    *(void**)(&fill_matrix_consecutive) = dlsym(library, "fill_matrix_consecutive");
    if (!fill_matrix_consecutive) {
        dlclose(library);
        return FAILED_TO_JOIN_FUNC;
    }

    *(void**)(&calculate_diagonal_sum) = dlsym(library, "calculate_diagonal_sum");
    if (!calculate_diagonal_sum) {
        dlclose(library);
        return FAILED_TO_JOIN_FUNC;
    }

    // =============================================================================
    //                                  Time test
    // =============================================================================

    triangle_matrix* matrix = create_matrix(matrix_size);
    if (!matrix) {
        dlclose(library);
        return MEMORY_ALLOCATION_FAILED;
    }
    if (fill_matrix_consecutive(matrix)) {
        free_matrix(matrix);
        dlclose(library);
        return EXECUTION_ERROR;
    }

    unsigned long int diagonal_sum = 0;
    double time_sum = 0;
    double time_min = RAND_MAX;
    double time_max = 0;
    bool errflag = false;

    for (size_t i = 0; i < number_of_iterations; ++i) {
        double time = get_time();

        if (calculate_diagonal_sum(matrix, &diagonal_sum, number_of_threads)) {
            errflag = true;
            break;
        }

        time = get_time() - time;
        if (time < time_min) {
            time_min = time;
        }
        if (time > time_max) {
            time_max = time;
        }
        time_sum += time;
    }

    if (errflag) {
        free_matrix(matrix);
        dlclose(library);
        return EXECUTION_ERROR;
    }

    char test_type[30] = "parallel";
    sprintf(test_type + 8, " %zu threads", number_of_threads);

    time_sum = time_sum / number_of_iterations;
    time_stat stat = {.iterations = number_of_iterations,
                      .test_type = test_type,
                      .min = time_min,
                      .max = time_max,
                      .avg = time_sum,
                      .matrix_size = matrix->size};
    save_stat(stat, diagonal_sum, "time_stat.txt");

    free_matrix(matrix);
    dlclose(library);

    pthread_exit(0);
}
