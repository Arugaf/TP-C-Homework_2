//
// Created by arugaf on 21.03.2020.
//

#include "stress_test.h"
#include "triangle_matrix.h"

#define EXECUTION_ERROR -3

#define DEFAULT_NUMBER_OF_ITERATIONS 10000

int main(int argc, char** argv) {
    size_t number_of_iterations = DEFAULT_NUMBER_OF_ITERATIONS;
    size_t matrix_size = 0;

    if (argc < 3) {
        return EXECUTION_ERROR;
    }

    matrix_size = strtol(argv[1], NULL, 10);
    if (matrix_size < 1) {
        return INVALID_DATA;
    }

    const char* matrix_filename = argv[2];
    FILE* matrix_file = fopen(matrix_filename, "r");
    if (!matrix_file) {
        return EXECUTION_ERROR;
    }

    if (argc > 3) {
        number_of_iterations = strtol(argv[3], NULL, 10);
        if (number_of_iterations < 1) {
            number_of_iterations = DEFAULT_NUMBER_OF_ITERATIONS;
        }
    }

    triangle_matrix* matrix = create_matrix(matrix_size);
    if (!matrix) {
        return MEMORY_ALLOCATION_FAILED;
    }

    if(fill_matrix(matrix, matrix_file)) {
        free_matrix(&matrix);
        return EXECUTION_ERROR;
    }

    long int diagonal_sum = 0;
    double time_avg = 0;
    double time_min = RAND_MAX;
    double time_max = 0;
    bool errflag = false;

    for (size_t i = 0; i < number_of_iterations; ++i) {
        double time = get_time();

        if (calculate_diagonal_sum(matrix, &diagonal_sum)) {
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
        time_avg += time;
    }

    if (errflag) {
        free_matrix(&matrix);
        return EXECUTION_ERROR;
    }

    char test_type[30] = "consecutive";

    time_avg = time_avg / number_of_iterations;
    time_stat stat = {.iterations = number_of_iterations,
            .test_type = test_type,
            .min = time_min,
            .max = time_max,
            .avg = time_avg,
            .matrix_size = matrix->size};
    save_stat(stat, diagonal_sum, "statistic.txt");

    free_matrix(&matrix);

    return 0;
}
