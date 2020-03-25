//
// Created by arugaf on 17.03.2020.
//

#ifndef TRIANGLE_MATRIX_TRIANGLE_MATRIX_H
#define TRIANGLE_MATRIX_TRIANGLE_MATRIX_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum errors {SUCCESS = 0,
             INVALID_POINTER = 2,
             INVALID_DATA = 3,
             THREAD_CREATION_ERROR = 4,
             MEMORY_ALLOCATION_FAILED = 5,
             INVALID_STREAM = 6,
             INVALID_PART = -1};

typedef struct {
    size_t size;
    unsigned char* elements;
} triangle_matrix;

triangle_matrix* create_matrix(size_t size);
int free_matrix(triangle_matrix** matrix);

int fill_matrix(triangle_matrix* matrix, FILE* stream);

int calculate_diagonal_sum(triangle_matrix* matrix, long int* result);

#endif //TRIANGLE_MATRIX_TRIANGLE_MATRIX_H
