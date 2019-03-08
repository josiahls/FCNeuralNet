#include <utility>

//
// Created by Laivins, Josiah on 2019-03-07.
//

#include <string>
#include <random>
#include "Layer.h"
#include "../linearMath/Matrix2d.h"

Layer::Layer(int size, int nextLayerSize, int seed, std::string randomWeightMode)
    : size(size), nextLayerSize(nextLayerSize), seed(seed), randomWeightMode(std::move(randomWeightMode)) {
    // Initialize a struct for handling layer dimensions
    Dimension d = {
            .currentLayerSize = size,
            .nextLayerSize = nextLayerSize
    };
    // Initialize the weight matrix
    this->w = Layer::getWeightInitialization(d, std::string());
    this->a = Layer::getWeightInitialization(d, "zeros");
    this->z = Layer::getWeightInitialization(d, "zeros");
    this->gradientW = this->w;
}

Matrix2d<float> Layer::getWeightInitialization(Dimension dimension, std::string mode) {
    // Define the local variable mode.
    mode = mode == ""? randomWeightMode: mode;
    // Create the 2d matrix. It is going to default to zero
    Matrix2d<float> m = Matrix2d<float>(dimension.currentLayerSize, dimension.nextLayerSize, 0);
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
        std::uniform_real_distribution<float> distribution(0,1);
        bind = std::bind(distribution, generator);
    } else if (mode == "middle") {
        bind = std::bind([](float a){return a; }, .5);
    } else if (mode == "glorot") {
        float limit = sqrt(6.f / this->w.size);
        std::uniform_real_distribution<float> distribution(-limit, limit);
        bind = std::bind(distribution, generator);
    }

    // Initialize the matrix with the determined random distribution
    for(int row = 0; row < m.nrows; row++){
        for(int col = 0; col < m.ncols; col++){
            m[row][col] = bind();
        }
    }

    return m;
}

std::vector<float> Layer::unwrap() {
    return w.unwrap();
}
