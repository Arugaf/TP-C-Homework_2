//
// Created by arugaf on 17.03.2020.
//

#include "triangle_matrix.h"

int calculate_diagonal_sum(triangle_matrix* matrix, long int* result) {
    if (!matrix || !matrix->elements || !result) {
        return INVALID_POINTER;
    }

    *result = 0;

    unsigned char* matrix_elements = matrix->elements;

    for (size_t i = 0; i < matrix->size; ++i) {
        *result += matrix_elements[i] - '0';
    }

    return SUCCESS;
}
