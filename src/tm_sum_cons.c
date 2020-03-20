//
// Created by arugaf on 17.03.2020.
//

#include "triangle_matrix.h"

int calculate_diagonal_sum(triangle_matrix* matrix, unsigned long int* result, ...) {
    if (!matrix || !matrix->elements || !result) {
        return INVALID_POINTER;
    }

    *result = 0;
    for (size_t i = 0, j = 0; i < matrix->size; ++i, j += i) {
        *result += matrix->elements[i + j] - '0';
    }

    return SUCCESS;
}
