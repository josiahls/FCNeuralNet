#include <utility>

//
// Created by Laivins, Josiah on 2019-03-07.
//

#include <string>
#include <random>
#include <opencv2/core/mat.hpp>
#include <opencv/cxmisc.h>
#include "Layer.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"

Layer::Layer(int size, int nextLayerSize, int seed, std::string randomWeightMode)
    : size(size), nextLayerSize(nextLayerSize), seed(seed), randomWeightMode(std::move(randomWeightMode)) {
    // Initialize a struct for handling layer dimensions
    Dimension d = {
            .currentLayerSize = size,
            .nextLayerSize = nextLayerSize
    };
    this->d = d;
    // Initialize the weight matrix
    this->w = Layer::getWeightInitialization(d, std::string());
    this->a = Layer::getWeightInitialization(d, "zeros");
    this->z = Layer::getWeightInitialization(d, "zeros");

    this->gradientW = this->w;

}

cv::Mat Layer::getWeightInitialization(Dimension dimension, std::string mode) {
    // Define the local variable mode.
    mode = mode == ""? randomWeightMode: mode;
    // Handle random initialization
    std::default_random_engine generator(std::random_device{}());
    if (seed != -1) {
        // To help with debugging, the seed can be defined. The unit test cases take this and
        // set the seed to 0
        generator.seed(static_cast<unsigned int>(seed));
    }

    // The std::bind function returns an instance of type function.
    // As a note, this is a lambda function that returns a float
    // You need to do it this way as opposed to `auto bind = blah blah;`
    // if you want to reassign the bind variable.
    std::function<float()> bind = std::bind([](float a){return a; }, 0);

    if (mode == "random") {
        // Produce a random distribution of weights from 0 to 1
        std::uniform_real_distribution<float> distribution(-1,1);
        bind = std::bind(distribution, generator);
    } else if (mode == "middle") {
        bind = std::bind([](float a){return a; }, .5);
    } else if (mode == "glorot") {
        float limit = sqrt(6.f / (dimension.currentLayerSize * dimension.nextLayerSize));
        std::uniform_real_distribution<float> distribution(-limit, limit);
        bind = std::bind(distribution, generator);
    }


    // Initialize the matrix with the determined random distribution
    // Create the 2d matrix. It is going to default to zero
    cv::Mat m(dimension.currentLayerSize, dimension.nextLayerSize, cv::DataType<float>::type);
    cv::parallel_for_(cv::Range(0, m.rows*m.cols), [&](const cv::Range& range){
        for (int r = range.start; r < range.end; r++)
        {
            int i = r / m.cols;
            int j = r % m.cols;
            m.ptr<float>(i)[j] = bind();
        }
    });

    return m;
}

cv::Mat Layer::getForwardOutput(cv::Mat z) {
    // Formula: (z = XW)
    this->z = z;

    // TODO decide whether to return or make this void
    // We perform the activation function on it
    this->a = this->getActivationSigmoid(z);

    // Similar to the @ in python
    return this->a * this->w;
}

cv::Mat Layer::getActivationSigmoid(cv::Mat z) {
    cv::Mat newA;
    cv::Mat multipliedMat;
    cv::Mat dividedMat;
    // FORMULA 2: (a = f(z))
    cv::multiply(cv::Scalar(-1), z, multipliedMat);
    std::vector<float> actVect2z;
    for(int k=0; k<z.rows; ++k)
        for(int j=0; j<z.cols; ++j)
            actVect2z.push_back(z.at<float>(k, j));
    std::vector<float> actVect2multipliedMat;
    for(int k=0; k<multipliedMat.rows; ++k)
        for(int j=0; j<multipliedMat.cols; ++j)
            actVect2multipliedMat.push_back(multipliedMat.at<float>(k, j));

    cv::exp(multipliedMat, newA);
    cv::divide(cv::Scalar(1), (cv::Scalar(1) + newA), dividedMat);
    std::vector<float> actVect2newA;
    for(int k=0; k<newA.rows; ++k)
        for(int j=0; j<newA.cols; ++j)
            actVect2newA.push_back(newA.at<float>(k, j));
    std::vector<float> actVect2dividedMat;
    for(int k=0; k<dividedMat.rows; ++k)
        for(int j=0; j<dividedMat.cols; ++j)
            actVect2dividedMat.push_back(dividedMat.at<float>(k, j));
    return dividedMat;
}

cv::Mat Layer::getActivationSigmoidPrime(cv::Mat z) {
    // FORMULA 4: f'(z)
    cv::Mat expNegativeZ;
    cv::Mat multipliedMat;
    cv::multiply(cv::Scalar(-1), z, multipliedMat);
    // Get exp(-z)
    cv::exp(multipliedMat, expNegativeZ);

    cv::Mat expNegativeZToPower;
    cv::Mat temp = cv::Scalar(1) + expNegativeZ;
    cv::Mat newTemp;
    temp.convertTo(newTemp, CV_32F);
    cv::pow(newTemp, 2, expNegativeZToPower);

    cv::Mat dividedMat;
    cv::divide(expNegativeZ, expNegativeZToPower, dividedMat);
    return dividedMat;
}
