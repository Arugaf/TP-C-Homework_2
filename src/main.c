//
// Created by arugaf on 17.03.2020.
//

#include "triangle_matrix.h"

#include <time.h>

int main() {
    clock_t start = clock();

    triangle_matrix* matrix = create_matrix(1);
    fill_matrix_consecutive(matrix);

    unsigned long int h = 0;
    calculate_diagonal_sum(matrix, &h, 5);

    free_matrix(matrix);

    clock_t end = clock();

    printf("%f\n%ld", (double)(end - start) / CLOCKS_PER_SEC, h);

    return 0;
}
