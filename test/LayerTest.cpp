//
// Created by Laivins, Josiah on 2019-03-07.
//

#include <gtest/gtest.h>
#include <cmath>
#include "../src/layers/Layer.h"

TEST(Layer, checkFieldInitsWeights) {
    // Init a small layer with a 2d Matrix with a 0 seed
    // so that we get the same behavior every time.
    Layer layerThing = Layer(5, 5, 0, "zeros");
    ASSERT_EQ(layerThing.a[0][0], 0);
    ASSERT_EQ(layerThing.z[0][0], 0);
    ASSERT_EQ(layerThing.gradientW[0][0], 0);
}

TEST(Layer, checkFloatGlorotWeights) {
    // Init a small layer with a 2d Matrix with a 0 seed
    // so that we get the same behavior every time.
    Layer layerThing = Layer(5, 5, 0, "glorot");

    ASSERT_EQ(layerThing.unwrap().size(),
              layerThing.size*layerThing.nextLayerSize) << "The layer's unwrap method does not match the expected size.";

    std::vector<float> weights = layerThing.unwrap();

    float limit = sqrt(6.f / layerThing.w.size);
    // Check that the values all center around 0 where the limit is supposed to be
    // symmetric
    for (auto& item : weights) {
        ASSERT_NEAR(item, 0, limit);
    }
}

TEST(Layer, checkFloatMiddleWeights) {
    // Init a small layer with a 2d Matrix with a 0 seed
    // so that we get the same behavior every time.
    Layer layerThing = Layer(5, 5, 0, "middle");

    ASSERT_EQ(layerThing.unwrap().size(),
              layerThing.size*layerThing.nextLayerSize) << "The layer's unwrap method does not match the expected size.";

    std::vector<float> weights = layerThing.unwrap();
    // Check that the values all center around .5
    for (auto& item : weights) {
        ASSERT_FLOAT_EQ(item, .5);
    }
}

TEST(Layer, checkZeroedWeights) {
    // Init a small layer with a 2d Matrix with a 0 seed
    // so that we get the same behavior every time.
    Layer layerThing = Layer(5, 5, 0, "literally anything else");

    ASSERT_EQ(layerThing.unwrap().size(),
            layerThing.size*layerThing.nextLayerSize) << "The layer's unwrap method does not match the expected size.";

    std::vector<float> weights = layerThing.unwrap();
    for (auto& item : weights) {
        ASSERT_EQ(item, 0);
    }
}

TEST(Layer, checkFloatWeights) {
    // Init a small layer with a 2d Matrix with a 0 seed
    // so that we get the same behavior every time.
    Layer layerThing = Layer(5, 5, 0, "random");

    ASSERT_EQ(layerThing.unwrap().size(),
              layerThing.size*layerThing.nextLayerSize) << "The layer's unwrap method does not match the expected size.";

    std::vector<float> weights = layerThing.unwrap();
    // Check that the values all center around .5
    for (auto& item : weights) {
        ASSERT_NEAR(item, .5, .5);
    }
}