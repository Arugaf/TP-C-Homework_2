//
// Created by arugaf on 25.03.2020.
//

#include "stress_test.h"

#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 3) {
        return -1;
    }

    size_t size = strtol(argv[1], NULL, 10);
    const char* file_name = argv[2];

    if (create_random_matrix_file(file_name, size)) {
        return -1;
    }

    return 0;
}
