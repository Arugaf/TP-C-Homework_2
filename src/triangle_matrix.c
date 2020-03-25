//
// Created by arugaf on 20.03.2020.
//

#include <triangle_matrix.h>

static size_t calculate_matrix_size(size_t size);

triangle_matrix* create_matrix(size_t size) {
    if (!size) {
        return NULL;
    }

    triangle_matrix* matrix = malloc(sizeof(triangle_matrix));
    if (!matrix) {
        return NULL;
    }

    size_t matrix_size = calculate_matrix_size(size);

    matrix->elements = calloc(matrix_size, sizeof(unsigned char));

    if (!matrix->elements) {
        free(matrix);
        return NULL;
    }

    matrix->size = size;

    return matrix;
}

int free_matrix(triangle_matrix** matrix) {
    if (!matrix || !(*matrix)) {
        return INVALID_POINTER;
    }

    if (!(*matrix)->elements) {
        free(*matrix);
        return INVALID_POINTER;
    }

    free((*matrix)->elements);
    free(*matrix);

    *matrix = NULL;

    return SUCCESS;
}

int fill_matrix(triangle_matrix* matrix, FILE* stream) {
    if (!stream) {
        return INVALID_STREAM;
    }

    if (!matrix || !matrix->elements) {
        return INVALID_POINTER;
    }

    bool invalid_data = false;

    size_t matrix_size = matrix->size;

    // Заполняется последовательно сверху вниз, слева направо
    //
    // Сначала последовательно хранятся элементы диагонали, затем оставшиеся элементы строки сверху вниз
    //
    // Example:
    // Треугольная матрица:0
    //                     1 2
    //                     3 0 1
    //                     2 3 0 1
    // Хранится как: [0211][1][30][230]

    for (size_t i = 0, j = matrix_size, k = 0; i < matrix_size; ++i, k += i) {
        for (; j < matrix_size + k; ++j) {
            if (fscanf(stream, "%c", &matrix->elements[j]) != 1) {
                invalid_data = true;
                break;
            }
        }

        if (invalid_data) {
            break;
        }

        if (fscanf(stream, "%c", &matrix->elements[i]) != 1) {
            invalid_data = true;
            break;
        }
    }


    if (invalid_data) {
        return INVALID_DATA;
    }

    return SUCCESS;
}

static size_t calculate_matrix_size(size_t size) {
    // Сумма арифметической прогрессии
    return (size * (size + 1)) >> (size_t) 1;
}
