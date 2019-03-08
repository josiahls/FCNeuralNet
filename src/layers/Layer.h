//
// Created by Laivins, Josiah on 2019-03-07.
//

#ifndef NEURALNETDEMO_LAYER_H
#define NEURALNETDEMO_LAYER_H


#include <opencv2/core/mat.hpp>
#include "Dimension.h"

class Layer {
public:
    int size;
    int nextLayerSize;
    int seed;
    std::string randomWeightMode;
    cv::Mat w;
    cv::Mat a;
    cv::Mat z;
    cv::Mat gradientW;

    /**
     * Creates a layer class. It is tasked with handling forward and back prop.
     *
     * The constructor determines the weight init.
     * Possible inputs are:
     * - random
     * - glorot
     * - middle
     * - zeros <-- Default
     *
     * @param size
     * @param nextLayerSize
     * @param seed
     * @param randomWeightMode
     */
    Layer(int size, int nextLayerSize, int seed = -1, std::string randomWeightMode = "");

    /**
     * Inits the w matrix based on user input.
     *
     * The constructor determines the weight init.
     * Possible inputs are:
     * - random
     * - glorot
     * - middle
     *
     * @param dimension
     * @return
     */
    cv::Mat getWeightInitialization(Dimension dimension, std::string mode = "");

    cv::Mat getForwardOutput(cv::Mat z);

    cv::Mat getActivationSigmoid(cv::Mat z);

    cv::Mat getActivationSigmoidPrime(cv::Mat z);
};


#endif //NEURALNETDEMO_LAYER_H
