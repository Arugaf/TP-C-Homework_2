//
// Created by arugaf on 17.03.2020.
//

#include "triangle_matrix.h"
#include <time.h>

int main() {
    clock_t start = clock();

    triangle_matrix* matrix = create_matrix(DEFAULT_MATRIX_SIZE);
    fill_matrix_consecutive(matrix);
    printf("%ld\n", calculate_diagonal_sum(matrix));
    free_matrix(matrix);

    clock_t end = clock();

    printf("%f", (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
