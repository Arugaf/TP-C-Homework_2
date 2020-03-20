//
// Created by arugaf on 17.03.2020.
//

#include "triangle_matrix.h"

unsigned long int calculate_diagonal_sum(triangle_matrix* matrix) {
    if (!matrix || !matrix->elements) {
        return INVALID_POINTER;
    }

    unsigned long int result = 0;

    for (size_t i = 0, j = 0; i < matrix->size; ++i, j += i) {
        result += matrix->elements[i + j] - '0';
    }

    return result;
}
