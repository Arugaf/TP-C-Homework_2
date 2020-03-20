//
// Created by arugaf on 20.03.2020.
//

#include "triangle_matrix.h"

#include <pthread.h>
#include <stdarg.h>
#include <unistd.h>

typedef struct {
    size_t begin_i;
    size_t begin_j;
    size_t end;
    triangle_matrix* matrix;
} matrix_part;

static void* calculate_sum_thread(void* thread_matrix_part) {
    unsigned long int result = 0;

    for (size_t i = ((matrix_part*) thread_matrix_part)->begin_i, j = ((matrix_part*) thread_matrix_part)->begin_j;
                                                i < ((matrix_part*) thread_matrix_part)->end; ++i, j += i) {
        result += ((matrix_part*) thread_matrix_part)->matrix->elements[i + j] - '0';
    }
    return (void*) result;
}

static matrix_part* split_matrix(triangle_matrix* matrix, size_t number_of_threads) {
    if (!matrix || !matrix->elements) {
        return NULL;
    }

    matrix_part* matrix_parts = malloc(sizeof(matrix_part) * number_of_threads);
    if (!matrix_parts) {
        return NULL;
    }

    size_t begin = 0;
    size_t end = matrix->size / number_of_threads;

    size_t part_size = (size_t) (matrix->size / number_of_threads);

    for (size_t i = 0; i < number_of_threads; ++i) {
        matrix_parts[i].begin_i = begin;
        matrix_parts[i].begin_j = (begin * begin + begin) >> (size_t) 1;
        matrix_parts[i].end = end;
        matrix_parts[i].matrix = matrix;

        begin = end;
        end += part_size;
    }

    size_t remainder = matrix->size % number_of_threads;
    if (remainder) {
        matrix_parts[number_of_threads - 1].end += remainder;
    }

    return matrix_parts;
}

int calculate_diagonal_sum(triangle_matrix* matrix, unsigned long int* result, ...) {
    if (!matrix || !matrix->elements || !result) {
        return INVALID_POINTER;
    }

    // Получаем количество потоков
    va_list list;
    va_start(list, result);
    size_t number_of_threads = va_arg(list, size_t);
    va_end(list);

    if (!number_of_threads) {
        return INVALID_DATA;
    }
    
    pthread_t* threads = malloc(sizeof(pthread_t) * number_of_threads);
    if (!threads) {
        return MEMORY_ALLOCATION_FAILED;
    }

    void** sums = malloc(sizeof(void*) * number_of_threads);
    if (!sums) {
        free(threads);
        return MEMORY_ALLOCATION_FAILED;
    }

    matrix_part* matrix_parts = split_matrix(matrix, number_of_threads);

    bool errflag = false;

    for (size_t i = 0; i < number_of_threads; ++i) {
        if (pthread_create(&threads[i], NULL, calculate_sum_thread, (void*) &(matrix_parts[i]))) {
            errflag = true;
        }

        if (pthread_join(threads[i], &(sums[i]))) {
            errflag = true;
        }

        if (errflag) {
            break;
        }
    }

    if (errflag) {
        free(threads);
        free(sums);
        free(matrix_parts);
        return THREAD_CREATION_ERROR;
    }

    *result = 0;
    for (size_t i = 0; i < number_of_threads; ++i) {
        *result += (unsigned long int)(sums[i]);
    }

    free(threads);
    free(sums);
    free(matrix_parts);

    return SUCCESS;
}
