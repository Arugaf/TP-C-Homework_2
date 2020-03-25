//
// Created by arugaf on 20.03.2020.
//

#include "triangle_matrix.h"

#include <pthread.h>
#include <signal.h>
#include <sys/sysinfo.h>

typedef struct {
    size_t begin;
    size_t end;
    triangle_matrix* matrix;
} matrix_part;

static void* calculate_sum_thread(void* thread_matrix_part) {
    if (!thread_matrix_part) {
        return (void*) INVALID_PART;
    }

    long int result = 0;

    size_t begin = ((matrix_part*) thread_matrix_part)->begin;
    size_t end = ((matrix_part*) thread_matrix_part)->end;
    unsigned char* matrix_elements = ((matrix_part*) thread_matrix_part)->matrix->elements;

    for (size_t i = begin; i < end; ++i) {
        result += matrix_elements[i] - '0';
    }

    return (void*) result;
}

static matrix_part* split_matrix(triangle_matrix* matrix, size_t number_of_threads) {
    if (!matrix || !matrix->elements || !number_of_threads) {
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
        matrix_parts[i].begin = begin;
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

int calculate_diagonal_sum(triangle_matrix* matrix, long int* result) {
    if (!matrix || !matrix->elements || !result) {
        return INVALID_POINTER;
    }

    int number_of_threads = get_nprocs();

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

            for (size_t j = 0; j < i; ++j) {
                pthread_kill(threads[j], 9);
            }
            break;
        }
    }

    if (errflag) {
        free(threads);
        free(sums);
        free(matrix_parts);
        return THREAD_CREATION_ERROR;
    }

    for (size_t i = 0; i < number_of_threads; ++i) {
        if (pthread_join(threads[i], &(sums[i]))) {
            errflag = true;
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
        if ((long int)sums == INVALID_PART) {
            break;
        }
        *result += (long int)(sums[i]);
    }

    free(threads);
    free(sums);
    free(matrix_parts);

    return SUCCESS;
}
