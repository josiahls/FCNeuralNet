//
// Created by Laivins, Josiah on 2019-03-05.
//

#include <opencv2/core/mat.hpp>
#include <opencv/cxmisc.h>
#include "vector"
#include "gtest/gtest.h"

/**
 * End to end test of the Matrix struct
 */
TEST(Matrix2d, Matrix2dCopyAndNumberTest) {

    cv::Mat matrix(200, 500, cv::DataType<float>::type);
    matrix = cv::Scalar(0);
    cv::Mat matrixCopy;
    matrix.copyTo(matrixCopy);

    // Test the sizes
    ASSERT_EQ(matrix.rows, 200) << "The rows do not match";
    ASSERT_EQ(matrix.cols, 500) << "The columns do not match";
    // Test the sizes
    ASSERT_EQ(matrixCopy.rows, 200) << "The rows do not match";
    ASSERT_EQ(matrixCopy.cols, 500) << "The columns do not match";
    // Test other params
    ASSERT_EQ(matrix.at<int>(0, 0), matrixCopy.at<int>(0, 0)) << "The default values do not match";
    ASSERT_EQ(matrix.size, matrixCopy.size) << "The sizes do not match";
    // Test valid bounds
    ASSERT_NO_THROW(matrixCopy.at<int>(199, 499));
    ASSERT_NO_THROW(matrix.at<int>(199, 499));

    // Test value assignment
    matrix.at<int>(0,0) = 5;
    ASSERT_NE(matrix.at<int>(0, 0), matrixCopy.at<int>(0, 0));
    matrixCopy.at<int>(0,0) = 6;
    ASSERT_NE(matrix.at<int>(0, 0), matrixCopy.at<int>(0, 0));
}

TEST(Matrix2d, Matrix2dForEachTest) {
    cv::Mat m(5, 5, cv::DataType<int>::type);
    cv::setNumThreads(1);
    cv::parallel_for_(cv::Range(0, m.rows*m.cols), [&](const cv::Range& range){
        for (int r = range.start; r < range.end; r++)
        {
            int i = r / m.cols;
            int j = r % m.cols;
            m.ptr<int>(i)[j] = 5;
        }
    });
    cv::parallel_for_(cv::Range(0, m.rows*m.cols), [&](const cv::Range& range){
        for (int r = range.start; r < range.end; r++)
        {
            int i = r / m.cols;
            int j = r % m.cols;
            ASSERT_EQ(m.ptr<int>(i)[j], 5);
        }
    });
    cv::setNumThreads(0);
}