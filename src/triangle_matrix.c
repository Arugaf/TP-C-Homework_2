//
// Created by arugaf on 20.03.2020.
//

#include <triangle_matrix.h>

static size_t calculate_matrix_size(size_t size);

triangle_matrix* create_matrix(const size_t size) {
    if (!size) {
        return NULL;
    }

    triangle_matrix* matrix = malloc(sizeof(triangle_matrix));
    if (!matrix) {
        return NULL;
    }

    size_t matrix_size = calculate_matrix_size(size);

    matrix->elements = calloc(matrix_size, sizeof(__uint8_t));

    if (!matrix->elements) {
        free(matrix);
        return NULL;
    }

    matrix->size = size;

    return matrix;
}

int free_matrix(triangle_matrix* matrix) {
    if (!matrix) {
        return INVALID_POINTER;
    }

    if (!matrix->elements) {
        free(matrix);
        return INVALID_POINTER;
    }

    free(matrix->elements);
    free(matrix);

    return SUCCESS;
}

int fill_matrix(triangle_matrix* matrix, FILE* stream) {
    if (!matrix || !matrix->elements) {
        return INVALID_POINTER;
    }

    bool invalid_data = false;

    size_t matrix_size = calculate_matrix_size(matrix->size);

    for (size_t i = 0; i < matrix_size; ++i) {
        if(fscanf(stream, "%c", &matrix->elements[i]) != 1) {
            invalid_data = true;
            break;
        }

        if (matrix->elements[i] < '0' || matrix->elements[i] > '3') {
            invalid_data = true;
            break;
        }
    }

    if (invalid_data) {
        return INVALID_DATA;
    }

    return SUCCESS;
}

int fill_matrix_consecutive(triangle_matrix* matrix) {
    if (!matrix || !matrix->elements) {
        return INVALID_POINTER;
    }

    size_t matrix_size = calculate_matrix_size(matrix->size);

    for (size_t i = 0; i < matrix_size; ++i) {
        matrix->elements[i] = i % 4 + '0';
    }

    return SUCCESS;
}

static size_t calculate_matrix_size(size_t size) {
    // Сумма арифметической прогрессии
    return (size * size + size) >> (size_t) 1;
}
