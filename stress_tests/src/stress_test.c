//
// Created by arugaf on 21.03.2020.
//

#include "stress_test.h"
#include "triangle_matrix.h"

#define NUMBER_OF_POSSIBLE_VALUES 4

double get_time() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    return (double) time.tv_sec * MS_IN_SEC + time.tv_nsec / NS_IN_MS;
}

void save_stat(time_stat stat, unsigned long int result, const char* filename) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        return;
    }

    fprintf(file, "=================================================================\n");
    fprintf(file, "\tTEST TYPE: %s\n", stat.test_type);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tNUMBER OF ITERATIONS: %zu\n", stat.iterations);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tMATRIX SIZE: %zu\n", stat.matrix_size);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tDIAGONAL SUM: %zu\n", result);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tMIN TIME: %lf ms\n", stat.min);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tMAX TIME: %lf ms\n", stat.max);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tAVG TIME: %lf ms\n", stat.avg);
    fprintf(file, "=================================================================\n\n");

    fclose(file);
}

static size_t calculate_matrix_size(size_t size) {
    return (size * (size + 1)) >> (size_t) 1;
}

int create_random_matrix_file(const char* file_name, size_t size) {
    FILE* matrix = fopen(file_name, "w");
    if (!matrix) {
        return INVALID_DATA;
    }

    size_t matrix_size = calculate_matrix_size(size);

    srandom(time(NULL));

    for (int i = 0; i < matrix_size; ++i) {
        unsigned char buf = (unsigned char) (random() % NUMBER_OF_POSSIBLE_VALUES + '0');
        fprintf(matrix,"%c",  buf);
    }

    fclose(matrix);

    return SUCCESS;
}

