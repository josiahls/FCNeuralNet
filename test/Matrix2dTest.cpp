//
// Created by Laivins, Josiah on 2019-03-05.
//

#include "vector"
#include "gtest/gtest.h"
#include "../src/linearMath/Matrix2d.h"
#include "../src/linearMath/Matrix2dv2.h"

/**
 * End to end test of the Matrix struct
 */
TEST(Matrix2d, Matrix2dCopyTest) {

    Matrix2d<int> matrix = Matrix2d<int>(200, 500, 0);
    Matrix2d<int> matrixCopy = matrix;
    // Test the sizes
    ASSERT_EQ(matrix.nrows, 200) << "The rows do not match";
    ASSERT_EQ(matrix.ncols, 500) << "The columns do not match";
    // Test the sizes
    ASSERT_EQ(matrixCopy.nrows, 200) << "The rows do not match";
    ASSERT_EQ(matrixCopy.ncols, 500) << "The columns do not match";
    // Test other params
    ASSERT_EQ(matrix.defaultValue, matrixCopy.defaultValue) << "The default values do not match";
    ASSERT_EQ(matrix.size, matrixCopy.size) << "The sizes do not match";
    // Test default values
    ASSERT_EQ(matrix[0][0], 0);
    ASSERT_EQ(matrix[199][0], 0);
    ASSERT_EQ(matrix[0][499], 0);
    ASSERT_EQ(matrix[199][499], 0);
    // Test default values
    ASSERT_EQ(matrixCopy[0][0], 0);
    ASSERT_EQ(matrixCopy[199][0], 0);
    ASSERT_EQ(matrixCopy[0][499], 0);
    ASSERT_EQ(matrixCopy[199][499], 0);
    // Test the matrix valid sizes
    ASSERT_ANY_THROW(matrix[199][600]);
    ASSERT_ANY_THROW(matrix[200][-600]);
    ASSERT_ANY_THROW(matrix[-200][600]);
    ASSERT_ANY_THROW(matrix[-4][-5]);
    // Test the matrix valid sizes
    ASSERT_ANY_THROW(matrixCopy[199][600]);
    ASSERT_ANY_THROW(matrixCopy[200][-600]);
    ASSERT_ANY_THROW(matrixCopy[-200][600]);
    ASSERT_ANY_THROW(matrixCopy[-4][-5]);
    // The money shot is we should be able to change the values in one,
    // and not affect the other.
    matrixCopy[0][0] = 5;
    ASSERT_NE(matrixCopy[0][0], matrix[0][0]);
    matrix[0][0] = 9;
    ASSERT_NE(matrixCopy[0][0], matrix[0][0]);
    ASSERT_EQ(matrixCopy[0][1], matrix[0][1]);


}

TEST(Matrix2d, Matrix2dIntTest) {

    Matrix2d<int> matrix = Matrix2d<int>(200, 500, 0);
    // Test the sizes
    ASSERT_EQ(matrix.nrows, 200) << "The rows do not match";
    ASSERT_EQ(matrix.ncols, 500) << "The columns do not match";
    // Test default values
    ASSERT_EQ(matrix[0][0], 0);
    ASSERT_EQ(matrix[199][0], 0);
    ASSERT_EQ(matrix[0][499], 0);
    ASSERT_EQ(matrix[199][499], 0);
    // Test the matrix valid sizes
    ASSERT_ANY_THROW(matrix[199][600]);
    ASSERT_ANY_THROW(matrix[200][-600]);
    ASSERT_ANY_THROW(matrix[-200][600]);
    ASSERT_ANY_THROW(matrix[-4][-5]);


}

TEST(Matrix2d, Matrix2dFloatTest) {
    Matrix2d<float> matrix = Matrix2d<float>(200, 500, 400.3);
    // Test the sizes
    ASSERT_EQ(matrix.nrows, 200) << "The rows do not match";
    ASSERT_EQ(matrix.ncols, 500) << "The columns do not match";
    // Test default values
    ASSERT_FLOAT_EQ(matrix[0][0], 400.3);
    ASSERT_FLOAT_EQ(matrix[199][0], 400.3);
    ASSERT_FLOAT_EQ(matrix[0][499], 400.3);
    ASSERT_FLOAT_EQ(matrix[199][499], 400.3);
    // Test the matrix valid sizes
    ASSERT_ANY_THROW(matrix[199][600]);
    ASSERT_ANY_THROW(matrix[200][-600]);
    ASSERT_ANY_THROW(matrix[-200][600]);
    ASSERT_ANY_THROW(matrix[-4][-5]);
}