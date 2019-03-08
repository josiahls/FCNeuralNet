//
// Created by Laivins, Josiah on 2019-03-07.
//

#ifndef NEURALNETDEMO_LAYER_H
#define NEURALNETDEMO_LAYER_H


#include "Dimension.h"
#include "../linearMath/Matrix2d.h"

class Layer {
public:
    int size;
    int nextLayerSize;
    int seed;
    std::string randomWeightMode;
    Matrix2d<float> w;
    Matrix2d<float> a;
    Matrix2d<float> z;
    Matrix2d<float> gradientW;

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
    Matrix2d<float> getWeightInitialization(Dimension dimension, std::string mode = "");

    Matrix2d<float> getForwardOutput(Matrix2d<float> z);

    /**
     * Unwraps Layer weights. Paired with a wrap function.
     *
     * @return
     */
    std::vector<float> unwrap();

    void wrap(std::vector<float> unwrappedMatrix, int otherNRows, int otherNCols);

//    Matrix2d<float> getActivationSigmoid(Matrix2d<float> matrix2d);
};


#endif //NEURALNETDEMO_LAYER_H
