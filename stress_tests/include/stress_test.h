//
// Created by arugaf on 21.03.2020.
//

#ifndef TRIANGLE_MATRIX_STRESS_TEST_H
#define TRIANGLE_MATRIX_STRESS_TEST_H

#include <stdio.h>
#include <time.h>

#define MS_IN_SEC 1e3
#define NS_IN_MS 1e6

typedef struct {
    char* test_type;
    size_t iterations;
    size_t matrix_size;
    double min;
    double max;
    double avg;
} time_stat;

double get_time();
void save_stat(time_stat stat, unsigned long int result, const char* filename);
int create_random_matrix_file(const char* file_name, size_t matrix_size);

#endif //TRIANGLE_MATRIX_STRESS_TEST_H
