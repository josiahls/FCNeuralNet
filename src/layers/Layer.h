//
// Created by Laivins, Josiah on 2019-03-07.
//

#ifndef NEURALNETDEMO_LAYER_H
#define NEURALNETDEMO_LAYER_H


#include <opencv2/core/mat.hpp>
#include "../utils/DebugHelpers.h"
#include "Dimension.h"

class Layer {
public:
    int size;
    int nextLayerSize;
    int seed;
    std::string activationType;
    Dimension d;
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
    Layer(int size, int nextLayerSize, int seed = -1, std::string randomWeightMode = "",
            std::string activation="sigmoid");

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

    /**
     * Updates the current activating values z and the activation a.
     *
     * @param z Values being used to currently activate the layer
     * @return The dot product of the activation matrix and weight matrix
     */
    cv::Mat getForwardOutput(cv::Mat z);

    /**
     * Gets the activation of a given z. Determined by the activationType
     * defined during the layer's creation.
     *
     * @param z Values being used to currently activate the layer
     * @return The activation
     */
    cv::Mat getActivation(cv::Mat z);

    /**
     * Gets the activation prime of a given z. Determined by the activationType
     * defined during the layer's creation.
     *
     * @param z Values being used to currently activate the layer
     * @return The activation prime.
     */
    cv::Mat getActivationPrime(cv::Mat z);

    cv::Mat getActivationTanh(cv::Mat z);

    cv::Mat getActivationTanhPrime(cv::Mat z);

    cv::Mat getActivationSigmoid(cv::Mat z);

    cv::Mat getActivationSigmoidPrime(cv::Mat z);
};


#endif //NEURALNETDEMO_LAYER_H
