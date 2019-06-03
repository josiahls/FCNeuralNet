/**
 * FCNeuralNet
 * Created by Laivins, Josiah https://josiahls.github.io/ on 2019-03-08
 *
 * This software is provided 'as-is', without any express or implied warranty.
 *
 * In no event will the author(s) be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "gtest/gtest.h"
#include "../src/nn/NeuralNet.h"


TEST(NeuralNet, NeuralNetTrainRMSETest) {
    NeuralNet nn = NeuralNet();
    nn.addLayer(2, 5, 0, "glorot", "tanh");
    nn.addLayer(5, 1, 0, "glorot", "tanh");
    nn.addLayer(1, 1, 0, "glorot", "tanh");

//    cv::Mat scaledX = (cv::Mat_<float>(3, 2) << 0.3, 0.5, 0.5, 0.1, 1, 0.2);
//    cv::Mat scaledY = (cv::Mat_<float>(3, 1) << 0.75, 0.93, 0.82);

    cv::Mat scaledX = (cv::Mat_<float>(5, 2) << 0.9, 0.9, 0.8, 0.8, 0.7, 0.7, 0.9, 0.7, .7, 0.9);
    cv::Mat scaledY = (cv::Mat_<float>(5, 1) << 0.7, 0.8, 0.9, 0.1, 0.2);

    nn.train(scaledX, scaledY);
    nn.train(scaledX, scaledY);
    nn.train(scaledX, scaledY);
    nn.train(scaledX, scaledY);
    cv::Mat prediction = nn.predict(scaledX);

    debug::ImshowMatrixDisplayer(nn.layers[0].w, std::tuple<int, int, int>(0, 0, 900), 10, false, false);
    debug::ImshowMatrixDisplayer(nn.layers[1].w, std::tuple<int, int, int>(0, 0, 900), 10, false, false);
    std::vector<float> y = debug::unwrapMat(prediction);
    std::vector<float> x = debug::unwrapMat(scaledX);
    std::vector<float> w1 = debug::unwrapMat(nn.layers[0].w);
    std::vector<float> w2 = debug::unwrapMat(nn.layers[1].w);
    ASSERT_NEAR(nn.rmse.back(), 0.004, 0.004);
}

TEST(NeuralNet, NeuralNetMinimizeAdamTest) {
    NeuralNet nn = NeuralNet();
    nn.addLayer(2, 3, 0, "middle", "sigmoid");
    nn.addLayer(3, 1, 0, "middle", "sigmoid");
    nn.addLayer(1, 1, 0, "middle", "sigmoid");

    cv::Mat scaledX = (cv::Mat_<float>(3, 2) << 0.3, 0.5, 0.5, 0.1, 1, 0.2);
    cv::Mat scaledY = (cv::Mat_<float>(3, 1) << 0.75, 0.82, 0.93);

    vector<float> unwrappedV = nn.costFunctionPrime(scaledX, scaledY);

    nn.minimizeAdamOptimizer(unwrappedV, scaledX, scaledY);
    /*
     * [ 1.623033    1.623033    1.623033
     * -0.52355991 -0.52355991 -0.52355991
        0.79052847  0.79052847  0.79052847]
     */
    vector<float> unwrappedWeights = nn.unwrap();
    ASSERT_NEAR(unwrappedWeights.at(0), 1.623033, 0.001);
    ASSERT_NEAR(unwrappedWeights.at(1), 1.623033, 0.001);
    ASSERT_NEAR(unwrappedWeights.at(2), 1.623033, 0.001);
    ASSERT_NEAR(unwrappedWeights.at(3), -0.52355991, 0.001);
    ASSERT_NEAR(unwrappedWeights.at(4), -0.52355991, 0.001);
    ASSERT_NEAR(unwrappedWeights.at(5), -0.52355991, 0.001);
    ASSERT_NEAR(unwrappedWeights.at(6), 0.79052847, 0.001);
    ASSERT_NEAR(unwrappedWeights.at(7), 0.79052847, 0.001);
    ASSERT_NEAR(unwrappedWeights.at(8), 0.79052847, 0.001);
}

TEST(NeuralNet, NeuralNetCostFunctionPrimeTest) {
    NeuralNet nn = NeuralNet();
    nn.addLayer(2, 3, 0, "middle", "sigmoid");
    nn.addLayer(3, 1, 0, "middle", "sigmoid");
    nn.addLayer(1, 1, 0, "middle", "sigmoid");

    cv::Mat scaledX = (cv::Mat_<float>(3, 2) << 0.3, 0.5, 0.5, 0.1, 1, 0.2);
    cv::Mat scaledY = (cv::Mat_<float>(3, 1) << 0.75, 0.82, 0.93);

    vector<float> unwrappedV = nn.costFunctionPrime(scaledX, scaledY);
    //[-0.00646678 -0.00646678 -0.00646678 -0.00172228 -0.00172228 -0.00172228
    // -0.0453134  -0.0453134  -0.0453134 ]
    ASSERT_NEAR(unwrappedV.at(0), -0.00646678, 0.001);
    ASSERT_NEAR(unwrappedV.at(1), -0.00646678, 0.001);
    ASSERT_NEAR(unwrappedV.at(2), -0.00646678, 0.001);
    ASSERT_NEAR(unwrappedV.at(3), -0.00172228, 0.001);
    ASSERT_NEAR(unwrappedV.at(4), -0.00172228, 0.001);
    ASSERT_NEAR(unwrappedV.at(5), -0.00172228, 0.001);
    ASSERT_NEAR(unwrappedV.at(6), -0.0453134, 0.001);
    ASSERT_NEAR(unwrappedV.at(7), -0.0453134, 0.001);
    ASSERT_NEAR(unwrappedV.at(8), -0.0453134, 0.001);
}

TEST(NeuralNet, NeuralNetCostFunctionTest) {
    NeuralNet nn = NeuralNet();
    nn.addLayer(2, 3, 0, "middle", "sigmoid");
    nn.addLayer(3, 1, 0, "middle", "sigmoid");
    nn.addLayer(1, 1, 0, "middle", "sigmoid");

    cv::Mat scaledX = (cv::Mat_<float>(3, 2) << 0.3, 0.5, 0.5, 0.1, 1, 0.2);
    cv::Mat scaledY = (cv::Mat_<float>(3, 1) << 0.75, 0.82, 0.93);

    double cost = nn.costFunction(scaledX, scaledY);
    ASSERT_NEAR(cost, 0.0286718714, 0.001);
}

TEST(NeuralNet, NeuralNetWrappingTest) {
    NeuralNet nn = NeuralNet();
    nn.addLayer(2, 3, 0, "middle");
    nn.addLayer(3, 1, 0, "middle");
    nn.addLayer(1, 1, 0, "middle");

    vector<float> unwrappedVector = nn.unwrap();

    ASSERT_FLOAT_EQ(unwrappedVector.size(), 9);
    ASSERT_FLOAT_EQ(unwrappedVector.at(8), .5);
    ASSERT_FLOAT_EQ(unwrappedVector.at(0), .5);
    nn.layers = vector<Layer>();
    nn.addLayer(2, 3, 0, "zeros");
    nn.addLayer(3, 1, 0, "middle");
    nn.addLayer(1, 1, 0, "zeros");
    vector<float> unwrappedZeroedVector = nn.unwrap();
    ASSERT_FLOAT_EQ(unwrappedZeroedVector.at(7), 0.5);
    ASSERT_FLOAT_EQ(unwrappedZeroedVector.at(0), 0);
    ASSERT_FLOAT_EQ(unwrappedZeroedVector.at(8), 0.5);
    nn.wrap(unwrappedVector);
    ASSERT_FLOAT_EQ(nn.layers.at(0).w.at<float>(0, 0), 0.5);
    ASSERT_FLOAT_EQ(nn.layers.at(0).w.at<float>(1, 0), 0.5);
    ASSERT_FLOAT_EQ(nn.layers.at(0).w.at<float>(0, 1), 0.5);
    ASSERT_FLOAT_EQ(nn.layers.at(0).w.at<float>(1, 1), 0.5);
    ASSERT_FLOAT_EQ(nn.layers.at(0).w.at<float>(0, 2), 0.5);
    ASSERT_FLOAT_EQ(nn.layers.at(0).w.at<float>(1, 2), 0.5);
    ASSERT_FLOAT_EQ(nn.layers.at(1).w.at<float>(0, 0), 0.5);
    ASSERT_FLOAT_EQ(nn.layers.at(1).w.at<float>(1, 0), 0.5);
    ASSERT_FLOAT_EQ(nn.layers.at(1).w.at<float>(2, 0), 0.5);
    ASSERT_FLOAT_EQ(nn.layers.at(2).w.at<float>(0, 0), 0);
}

TEST(NeuralNet, NeuralNetPredictTest) {
    NeuralNet nn = NeuralNet();
    nn.addLayer(2, 3, 0, "middle");
    nn.addLayer(3, 1, 0, "middle");
    nn.addLayer(1, 1, 0, "middle");

    cv::Mat testX = (cv::Mat_<float>(3, 2) << 0.3, 0.5, 0.5, 0.1, 1, 0.2);
    cv::Mat predY = nn.predict(testX);

    ASSERT_EQ(predY.rows, 3);
    ASSERT_EQ(predY.cols, 1);

//    debug::unwrapMat(predY);
    ASSERT_NEAR(predY.at<float>(0, 0), .43, .3);
    ASSERT_NEAR(predY.at<float>(1, 0), .43, .3);
    ASSERT_NEAR(predY.at<float>(2, 0), .43, .3);
}

TEST(NeuralNet, NeuralNetAddLayerTest) {
    NeuralNet nn = NeuralNet();
    nn.addLayer(2, 3, 0, "middle");
    nn.addLayer(3, 1, 0, "middle");
    nn.addLayer(1, 1, 0, "middle");
    ASSERT_EQ(nn.layers.size(), 3);
}

TEST(NeuralNet, NeuralNetForwardTest) {
    NeuralNet nn = NeuralNet();
    nn.addLayer(2, 3, 0, "middle");
    nn.addLayer(3, 1, 0, "middle");
    nn.addLayer(1, 1, 0, "middle");

    cv::Mat testX = (cv::Mat_<float>(3, 2) << 0.3, 0.5, 0.5, 0.1, 1, 0.2);
    cv::Mat predY = nn.forward(testX);

    ASSERT_EQ(predY.rows, 3);
    ASSERT_EQ(predY.cols, 1);

    ASSERT_NEAR(predY.at<float>(0, 0), .43, .3);
    ASSERT_NEAR(predY.at<float>(1, 0), .43, .3);
    ASSERT_NEAR(predY.at<float>(2, 0), .43, .3);
}