//
// Created by arugaf on 21.03.2020.
//

#include "time_test.h"

double get_time() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    return (double) time.tv_sec * MS_IN_SEC + time.tv_nsec / NS_IN_MS;
}

void save_stat(time_stat stat, unsigned long int result, const char* filename) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        return;
    }

    fprintf(file, "=================================================================\n");
    fprintf(file, "\tTEST TYPE: %s\n", stat.test_type);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tNUMBER OF ITERATIONS: %zu\n", stat.iterations);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tMATRIX SIZE: %zu\n", stat.matrix_size);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tDIAGONAL SUM: %zu\n", result);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tMIN TIME: %lf ms\n", stat.min);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tMAX TIME: %lf ms\n", stat.max);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "\tAVG TIME: %lf ms\n", stat.avg);
    fprintf(file, "=================================================================\n\n");

    fclose(file);
}

