//
// Created by arugaf on 21.03.2020.
//

extern "C" {
    #include "triangle_matrix.h"
}

#include "gtest/gtest.h"

TEST(MatrixCreation, IncorrectSize) {
    const size_t matrix_size = 0;
    EXPECT_EQ(create_matrix(matrix_size), nullptr);
}

TEST(MatrixCreation, CorrectData) {
    const size_t matrix_size = 10;
    triangle_matrix* matrix = create_matrix(matrix_size);

    ASSERT_TRUE(matrix);
    EXPECT_EQ(matrix->size, matrix_size);

    size_t matrix_array_size = (matrix_size * matrix_size + matrix_size) >> (size_t) 1;

    for (size_t i = 0; i < matrix_array_size; ++i) {
        EXPECT_EQ(matrix->elements[i], 0);
    }

    free_matrix(&matrix);
}

TEST(MatrixFreeing, NoMatrix) {
    EXPECT_EQ(free_matrix(nullptr), INVALID_POINTER);
}

TEST(MatrixFreeing, NoElements) {
    auto matrix = (triangle_matrix*)malloc(sizeof(triangle_matrix));
    matrix->elements = nullptr;
    EXPECT_EQ(free_matrix(&matrix), INVALID_POINTER);
}

TEST(MatrixFreeing, CorrectData) {
    const size_t matrix_size = 10;
    triangle_matrix* matrix = create_matrix(matrix_size);

    ASSERT_TRUE(matrix);

    EXPECT_EQ(free_matrix(&matrix), SUCCESS);
}

TEST(MatrixFilling, NoMatrix) {
    EXPECT_EQ(fill_matrix(nullptr, stdin), INVALID_POINTER);
}

TEST(MatrixFilling, NoElements) {
    auto matrix = (triangle_matrix*)malloc(sizeof(triangle_matrix));
    matrix->elements = nullptr;
    EXPECT_EQ(fill_matrix(matrix, stdin), INVALID_POINTER);
    free_matrix(&matrix);
}

TEST(MatrixFilling, InvalidData) {
    const size_t matrix_size = 2;
    triangle_matrix* matrix = create_matrix(matrix_size);

    ASSERT_TRUE(matrix);

    const char* filename = "filling_matrix_invalid_data";
    FILE* test_file = fopen(filename, "w");
    ASSERT_TRUE(test_file);

    fclose(test_file);

    test_file = fopen(filename, "r");
    ASSERT_TRUE(test_file);

    EXPECT_EQ(fill_matrix(matrix, test_file), INVALID_DATA);
    fclose(test_file);

    free_matrix(&matrix);
}

TEST(MatrixFilling, CorrectData) {
    const size_t matrix_size = 5;
    triangle_matrix* matrix = create_matrix(matrix_size);

    ASSERT_TRUE(matrix);

    const char* filename = "filling_matrix_correct_data.txt";
    FILE* test_file = fopen(filename, "w");
    ASSERT_TRUE(test_file);

    const unsigned char test_string[] = {'2',
                                         '3', '0',
                                         '2', '0', '3',
                                         '1', '0', '3', '2',
                                         '2', '2', '2', '3', '1'};
    const unsigned char control_string[] = {'2', '0', '3', '2', '1',
                                            '3',
                                            '2', '0',
                                            '1', '0', '3',
                                            '2', '2', '2', '3'};

    fprintf(test_file, "%s", test_string);
    fclose(test_file);

    test_file = fopen(filename, "r");
    ASSERT_TRUE(test_file);

    ASSERT_EQ(fill_matrix(matrix, test_file), SUCCESS);
    fclose(test_file);

    size_t matrix_array_size = (matrix_size * matrix_size + matrix_size) >> (size_t) 1;

    for (size_t i = 0; i < matrix_array_size; ++i) {
        EXPECT_EQ(matrix->elements[i], control_string[i]);
    }

    free_matrix(&matrix);
}


TEST(CalculateSum, NoMatrix) {
    EXPECT_EQ(calculate_diagonal_sum(nullptr, nullptr), INVALID_POINTER);
}

TEST(CalculateSum, CorrectData) {
    const size_t matrix_size = 5;
    const long int sum = 8;
    triangle_matrix* matrix = create_matrix(matrix_size);

    ASSERT_TRUE(matrix);

    const char* filename = "filling_matrix_correct_data.txt";
    FILE* test_file = fopen(filename, "w");
    ASSERT_TRUE(test_file);

    const unsigned char test_string[] = {'2',
                                         '3', '0',
                                         '2', '0', '3',
                                         '1', '0', '3', '2',
                                         '2', '2', '2', '3', '1'};

    fprintf(test_file, "%s", test_string);
    fclose(test_file);

    test_file = fopen(filename, "r");
    ASSERT_TRUE(test_file);

    ASSERT_EQ(fill_matrix(matrix, test_file), SUCCESS);

    long int result = 0;

    ASSERT_EQ(calculate_diagonal_sum(matrix, &result), SUCCESS);
    EXPECT_EQ(result, sum);

    free_matrix(&matrix);
}

