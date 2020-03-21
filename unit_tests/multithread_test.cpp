//
// Created by arugaf on 21.03.2020.
//

extern "C" {
#include "triangle_matrix.h"
}

#include "gtest/gtest.h"

#include <dlfcn.h>
#include <pthread.h>

class MatrixCreation : public ::testing::Test {
protected:
    void SetUp() override {
        library = dlopen("./libTriangle_matrix_dynamic.so", RTLD_LAZY);

        *(void**)(&create_matrix) = dlsym(library, "create_matrix");
        *(void**)(&free_matrix) = dlsym(library, "free_matrix");
    }

    void TearDown() override {
        dlclose(library);
    }

    void* library;

    void* (*create_matrix)(const size_t size);
    void* (*free_matrix)(triangle_matrix* matrix);
};

TEST_F(MatrixCreation, IncorrectSize) {
    const size_t matrix_size = 0;
    EXPECT_EQ(create_matrix(matrix_size), nullptr);
}

TEST_F(MatrixCreation, CorrectData) {
    const size_t matrix_size = 10;
    auto matrix = (triangle_matrix*)create_matrix(matrix_size);

    ASSERT_TRUE(matrix);
    EXPECT_EQ(matrix->size, matrix_size);

    size_t matrix_array_size = (matrix_size * matrix_size + matrix_size) >> (size_t) 1;

    for (size_t i = 0; i < matrix_array_size; ++i) {
        EXPECT_EQ(matrix->elements[i], 0);
    }

    free_matrix(matrix);
}

class MatrixFreeing : public ::testing::Test {
protected:
    void SetUp() override {
        library = dlopen("./libTriangle_matrix_dynamic.so", RTLD_LAZY);

        *(void**)(&create_matrix) = dlsym(library, "create_matrix");
        *(void**)(&free_matrix) = dlsym(library, "free_matrix");
    }

    void TearDown() override {
        dlclose(library);
    }

    void* library;

    void* (*create_matrix)(const size_t size);
    void* (*free_matrix)(triangle_matrix* matrix);
};

TEST_F(MatrixFreeing, NoMatrix) {
    EXPECT_EQ((long int)(free_matrix)(nullptr), INVALID_POINTER);
}

TEST_F(MatrixFreeing, NoElements) {
    auto matrix = (triangle_matrix*)malloc(sizeof(triangle_matrix));
    matrix->elements = nullptr;
    EXPECT_EQ((long int)free_matrix(matrix), INVALID_POINTER);
}

TEST_F(MatrixFreeing, CorrectData) {
    const size_t matrix_size = 10;
    auto matrix = (triangle_matrix*)create_matrix(matrix_size);

    ASSERT_TRUE(matrix);

    EXPECT_EQ((long int)free_matrix(matrix), SUCCESS);
}

class MatrixFilling : public ::testing::Test {
protected:
    void SetUp() override {
        library = dlopen("./libTriangle_matrix_dynamic.so", RTLD_LAZY);

        *(void**)(&create_matrix) = dlsym(library, "create_matrix");
        *(void**)(&free_matrix) = dlsym(library, "free_matrix");
        *(void**)(&fill_matrix) = dlsym(library, "fill_matrix");
    }

    void TearDown() override {
        dlclose(library);
    }

    void* library;

    void* (*create_matrix)(const size_t size);
    void* (*free_matrix)(triangle_matrix* matrix);
    void* (*fill_matrix)(triangle_matrix* matrix, FILE* stream);
};

TEST_F(MatrixFilling, NoMatrix) {
    EXPECT_EQ((long int)fill_matrix(nullptr, stdin), INVALID_POINTER);
}

TEST_F(MatrixFilling, NoElements) {
    auto matrix = (triangle_matrix*)malloc(sizeof(triangle_matrix));
    matrix->elements = nullptr;
    EXPECT_EQ((long int)fill_matrix(matrix, stdin), INVALID_POINTER);
    free(matrix);
}

TEST_F(MatrixFilling, InvalidData) {
    const size_t matrix_size = 2;
    auto matrix = (triangle_matrix*)create_matrix(matrix_size);

    ASSERT_TRUE(matrix);

    const char* filename = "filling_matrix_invalid_data";
    FILE* test_file = fopen(filename, "w");
    ASSERT_TRUE(test_file);

    fprintf(test_file, "1, 6");
    fclose(test_file);

    test_file = fopen(filename, "r");
    ASSERT_TRUE(test_file);

    EXPECT_EQ((long int)fill_matrix(matrix, test_file), INVALID_DATA);
    fclose(test_file);

    free_matrix(matrix);
}

TEST_F(MatrixFilling, CorrectData) {
    const size_t matrix_size = 5;
    auto matrix = (triangle_matrix*)create_matrix(matrix_size);

    ASSERT_TRUE(matrix);

    const char* filename = "filling_matrix_invalid_data.txt";
    FILE* test_file = fopen(filename, "w");
    ASSERT_TRUE(test_file);

    const __uint8_t test_string[] = {'2',
                                     '3', '0',
                                     '2', '0', '3',
                                     '1', '0', '3', '2',
                                     '2', '2', '2', '3', '1'};
    fprintf(test_file, "%s", test_string);
    fclose(test_file);

    test_file = fopen(filename, "r");
    ASSERT_TRUE(test_file);

    ASSERT_EQ((long int)fill_matrix(matrix, test_file), SUCCESS);
    fclose(test_file);

    size_t matrix_array_size = (matrix_size * matrix_size + matrix_size) >> (size_t) 1;

    for (size_t i = 0; i < matrix_array_size; ++i) {
        EXPECT_EQ(matrix->elements[i], test_string[i]);
    }

    free_matrix(matrix);
}

class MatrixFillingCons : public ::testing::Test {
protected:
    void SetUp() override {
        library = dlopen("./libTriangle_matrix_dynamic.so", RTLD_LAZY);

        *(void**)(&create_matrix) = dlsym(library, "create_matrix");
        *(void**)(&free_matrix) = dlsym(library, "free_matrix");
        *(void**)(&fill_matrix_consecutive) = dlsym(library, "fill_matrix_consecutive");
    }

    void TearDown() override {
        dlclose(library);
    }

    void* library;

    void* (*create_matrix)(const size_t size);
    void* (*free_matrix)(triangle_matrix* matrix);
    void* (*fill_matrix_consecutive)(triangle_matrix* matrix);
};

TEST_F(MatrixFillingCons, NoMatrix) {
    EXPECT_EQ((long int)fill_matrix_consecutive(nullptr), INVALID_POINTER);
}

TEST_F(MatrixFillingCons, NoElements) {
    auto matrix = (triangle_matrix*)malloc(sizeof(triangle_matrix));
    matrix->elements = nullptr;
    EXPECT_EQ((long int)fill_matrix_consecutive(matrix), INVALID_POINTER);
    free(matrix);
}

TEST_F(MatrixFillingCons, CorrectData) {
    const size_t matrix_size = 5;
    auto matrix = (triangle_matrix*)create_matrix(matrix_size);

    ASSERT_TRUE(matrix);

    const __uint8_t test_string[] = {'0',
                                     '1', '2',
                                     '3', '0', '1',
                                     '2', '3', '0', '1',
                                     '2', '3', '0', '1', '2'};

    ASSERT_EQ((long int)fill_matrix_consecutive(matrix), SUCCESS);

    size_t matrix_array_size = (matrix_size * matrix_size + matrix_size) >> (size_t) 1;

    for (size_t i = 0; i < matrix_array_size; ++i) {
        EXPECT_EQ(matrix->elements[i], test_string[i]);
    }

    free_matrix(matrix);
}

class CalculateSum : public ::testing::Test {
protected:
    void SetUp() override {
        library = dlopen("./libTriangle_matrix_dynamic.so", RTLD_LAZY);

        *(void**)(&create_matrix) = dlsym(library, "create_matrix");
        *(void**)(&free_matrix) = dlsym(library, "free_matrix");
        *(void**)(&fill_matrix_consecutive) = dlsym(library, "fill_matrix_consecutive");
        *(void**)(&calculate_diagonal_sum) = dlsym(library, "calculate_diagonal_sum");
    }

    void TearDown() override {
        dlclose(library);
    }

    void* library;

    void* (*create_matrix)(const size_t size);
    void* (*free_matrix)(triangle_matrix* matrix);
    void* (*fill_matrix_consecutive)(triangle_matrix* matrix);
    void* (*calculate_diagonal_sum)(triangle_matrix* matrix, unsigned long int* result, ...);
};

TEST_F(CalculateSum, NoMatrix) {
    EXPECT_EQ((long int)calculate_diagonal_sum(nullptr, nullptr), INVALID_POINTER);
}

TEST_F(CalculateSum, NoThreads) {
    const size_t matrix_size = 5;
    auto matrix = (triangle_matrix*)create_matrix(matrix_size);
    unsigned long int result = 0;

    ASSERT_TRUE(matrix);

    EXPECT_EQ((long int)calculate_diagonal_sum(matrix, &result, 0), INVALID_DATA);

    free_matrix(matrix);
}

TEST_F(CalculateSum, CorrectData1Thread) {
    const size_t matrix_size = 5;
    const long int sum = 6;
    unsigned long int result = 0;
    auto matrix = (triangle_matrix*)create_matrix(matrix_size);

    ASSERT_TRUE(matrix);
    ASSERT_EQ((long int)fill_matrix_consecutive(matrix), SUCCESS);

    ASSERT_EQ((long int)calculate_diagonal_sum(matrix, &result, 1), SUCCESS);
    EXPECT_EQ(result, sum);

    free_matrix(matrix);
}

TEST_F(CalculateSum, CorrectData2Threads) {
    const size_t matrix_size = 5;
    const long int sum = 6;
    unsigned long int result = 0;
    auto matrix = (triangle_matrix*)create_matrix(matrix_size);

    ASSERT_TRUE(matrix);
    ASSERT_EQ((long int)fill_matrix_consecutive(matrix), SUCCESS);

    ASSERT_EQ((long int)calculate_diagonal_sum(matrix, &result, 2), SUCCESS);
    EXPECT_EQ(result, sum);

    free_matrix(matrix);
}

TEST_F(CalculateSum, CorrectData4Threads) {
    const size_t matrix_size = 5;
    const long int sum = 6;
    unsigned long int result = 0;
    auto matrix = (triangle_matrix*)create_matrix(matrix_size);

    ASSERT_TRUE(matrix);
    ASSERT_EQ((long int)fill_matrix_consecutive(matrix), SUCCESS);

    ASSERT_EQ((long int)calculate_diagonal_sum(matrix, &result, 4), SUCCESS);
    EXPECT_EQ(result, sum);

    free_matrix(matrix);
}

TEST_F(CalculateSum, CorrectData8Threads) {
    const size_t matrix_size = 5;
    const long int sum = 6;
    unsigned long int result = 0;
    auto matrix = (triangle_matrix*)create_matrix(matrix_size);

    ASSERT_TRUE(matrix);
    ASSERT_EQ((long int)fill_matrix_consecutive(matrix), SUCCESS);

    ASSERT_EQ((long int)calculate_diagonal_sum(matrix, &result, 8), SUCCESS);
    EXPECT_EQ(result, sum);

    free_matrix(matrix);
}

TEST_F(CalculateSum, CorrectData15Threads) {
    const size_t matrix_size = 5;
    const long int sum = 6;
    unsigned long int result = 0;
    auto matrix = (triangle_matrix*)create_matrix(matrix_size);

    ASSERT_TRUE(matrix);
    ASSERT_EQ((long int)fill_matrix_consecutive(matrix), SUCCESS);

    ASSERT_EQ((long int)calculate_diagonal_sum(matrix, &result, 15), SUCCESS);
    EXPECT_EQ(result, sum);

    free_matrix(matrix);
}
