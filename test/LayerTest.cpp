//
// Created by Laivins, Josiah on 2019-03-07.
//

#include <gtest/gtest.h>
#include <cmath>
#include <opencv/cxmisc.h>
#include "../src/layers/Layer.h"


TEST(Layer, checkForward) {
    Layer layer = Layer(3, 2, 0, "middle");
    layer.w = (cv::Mat_<float>(2, 3) << -0.90937, 1.02899, 0.57227, 0.73312, -0.21868, 0.24272);
    cv::Mat testMat = (cv::Mat_<float>(3, 2) << 0.3, 0.5, 0.5, 0.1, 1, 0.2);
    cv::Mat result = layer.getForwardOutput(testMat);

    ASSERT_NEAR(result.at<float>(0,0), -0.06604, 0.001);
    ASSERT_NEAR(result.at<float>(0,1), 0.45497, 0.001);
    ASSERT_NEAR(result.at<float>(0,2), 0.47982, 0.001);
    ASSERT_NEAR(result.at<float>(1,0), -0.18117, 0.001);
    ASSERT_NEAR(result.at<float>(1,1), 0.5257, 0.001);
    ASSERT_NEAR(result.at<float>(1,2), 0.48364, 0.001);
    ASSERT_NEAR(result.at<float>(2,0), -0.2617, 0.001);
    ASSERT_NEAR(result.at<float>(2,1), 0.63205, 0.001);
    ASSERT_NEAR(result.at<float>(2,2), 0.55182, 0.001);
}

TEST(Layer, checkActivationSigmoidPrime) {
    Layer layer = Layer(5, 5, 0, "middle");
    cv::Mat testMat = (cv::Mat_<float>(3, 1) << -0.2810396, -0.407007, -0.405505);
    cv::Mat result = layer.getActivationSigmoidPrime(testMat);

    ASSERT_NEAR(result.at<float>(0,0), 0.245, 0.001);
    ASSERT_NEAR(result.at<float>(1,0), 0.239, 0.001);
    ASSERT_NEAR(result.at<float>(2,0), 0.239, 0.001);
}

TEST(Layer, checkActivationSigmoid) {
    Layer layer = Layer(5, 5, 0, "middle");
    cv::Mat testMat = (cv::Mat_<float>(3, 2) << 0.3, 0.5, 0.5, 0.1, 1, 0.2);
    cv::Mat result = layer.getActivationSigmoid(testMat);

    ASSERT_NEAR(result.at<float>(0,0), 0.57444, 0.001);
    ASSERT_NEAR(result.at<float>(0,1), 0.62245, 0.001);
    ASSERT_NEAR(result.at<float>(1,0), 0.62245, 0.001);
    ASSERT_NEAR(result.at<float>(1,1), 0.52497, 0.001);
    ASSERT_NEAR(result.at<float>(2,0), 0.73105, 0.001);
    ASSERT_NEAR(result.at<float>(2,1), 0.54983, 0.001);
}

TEST(Layer, checkFieldInitsWeights) {
    // Init a small layer with a 2d Matrix with a 0 seed
    // so that we get the same behavior every time.
    Layer layer = Layer(5, 5, 0, "zeros");
    ASSERT_EQ(layer.a.at<int>(0,0), 0);
    ASSERT_EQ(layer.z.at<int>(0,0), 0);
    ASSERT_EQ(layer.gradientW.at<int>(0,0), 0);
}

TEST(Layer, checkFieldInitsWeightsRandom) {
    Layer layer = Layer(5, 5, 0, "random");
    cv::parallel_for_(cv::Range(0, layer.w.rows*layer.w.cols), [&](const cv::Range& range){
        for (int r = range.start; r < range.end; r++)
        {
            int i = r / layer.w.cols;
            int j = r % layer.w.cols;
            ASSERT_NEAR(layer.w.ptr<float>(i)[j], .5, .5);
        }
    });
}

TEST(Layer, checkFieldInitsWeightsGlorot) {
    Layer layer = Layer(5, 5, 0, "glorot");
    float limit = sqrt(6.f / (layer.w.rows * layer.w.cols));
    cv::parallel_for_(cv::Range(0, layer.w.rows*layer.w.cols), [&](const cv::Range& range){
        for (int r = range.start; r < range.end; r++)
        {
            int i = r / layer.w.cols;
            int j = r % layer.w.cols;
            ASSERT_NEAR(layer.w.ptr<float>(i)[j], 0, limit);
        }
    });
}

TEST(Layer, checkFieldInitsWeightsMiddle) {
    Layer layer = Layer(5, 5, 0, "middle");
    float limit = sqrt(6.f / (layer.w.rows * layer.w.cols));
    cv::parallel_for_(cv::Range(0, layer.w.rows*layer.w.cols), [&](const cv::Range& range){
        for (int r = range.start; r < range.end; r++)
        {
            int i = r / layer.w.cols;
            int j = r % layer.w.cols;
            ASSERT_FLOAT_EQ(layer.w.ptr<float>(i)[j], .5);
        }
    });
}