//
// Created by arugaf on 17.03.2020.
//

#ifndef TRIANGLE_MATRIX_TRIANGLE_MATRIX_H
#define TRIANGLE_MATRIX_TRIANGLE_MATRIX_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define INVALID_POINTER -2
#define INVALID_DATA -3

#define DEFAULT_MATRIX_SIZE 100000

typedef struct {
    size_t size;
    __uint8_t* elements;
} triangle_matrix;

triangle_matrix* create_matrix(size_t size);
int free_matrix(triangle_matrix* matrix);

size_t calculate_matrix_size(size_t size);

int fill_matrix(triangle_matrix* matrix, FILE* stream);
int fill_matrix_consecutive(triangle_matrix* matrix);

unsigned long int calculate_diagonal_sum(triangle_matrix* matrix);

#endif //TRIANGLE_MATRIX_TRIANGLE_MATRIX_H
