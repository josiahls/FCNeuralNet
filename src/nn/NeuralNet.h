#include <utility>

//
// Created by Laivins, Josiah on 2019-03-05.
//

#ifndef NEURALNETDEMO_NEURALNET_H
#define NEURALNETDEMO_NEURALNET_H

#include <iostream>
#include <vector>
#include "../layers/Layer.h"
#include <opencv2/core/mat.hpp>
#include <opencv/cxmisc.h>


using namespace std;


class NeuralNet {
public:
    vector<Layer> layers;
    vector<float> rmse;
    vector<float> rmseValidate;
    vector<float> cost;
    int maxY;
    int maxX;

    NeuralNet() {
        this->layers = vector<Layer>();
        this->rmse = vector<float>();
        this->rmseValidate = vector<float>();
        this->cost = vector<float>();
        this->maxY = 0;
        this->maxX = 0;
    };

    cv::Mat predict(const cv::Mat &x) {
        cv::Mat y = forward(x);
        return y;
    };

    double costFunction(const cv::Mat &x, const cv::Mat &y) {
        // FORMULA 3: J = sum((1/2) * (y - y_hat)^2)
        cv::Mat predY = forward(x);

        cv::Mat subtractionToPower;
        cv::pow(predY - y, 2, subtractionToPower);
        return cv::sum(subtractionToPower)[0] * 0.5;
    }

    void train(const cv::Mat &x, const cv::Mat &y, int numIterations = 15000, int batchSize = -1) {
        vector<float> paramsInitial = unwrap();
        vector<float> unwrappedV = costFunctionPrime(x, y);

        minimizeAdamOptimizer(unwrappedV, x, y, numIterations);

        if (batchSize == -1) {
            logRMSE(x, y);
        }
    }

    void logRMSE(const cv::Mat &x, const cv::Mat &y) {
        cv::Mat predY = forward(x);
        // Calc the RMSE
        cv::Scalar scalarMean = cv::mean(predY - y);
        float locRMSE = cv::sqrt(cv::pow((float)scalarMean[0], 2.f));
        // Log that RMSE
        rmse.emplace_back(locRMSE);
    }

    void logBatchRMSE(const cv::Mat &predY, const cv::Mat &y) {
        cv::Scalar scalarMean = cv::mean(predY - y);
        float locRMSE = cv::sqrt(cv::pow((float)scalarMean[0], 2.f));
        // Log that RMSE
        rmse.emplace_back(locRMSE);
    }

    void logBatchRMSEValidation(const cv::Mat &predY, const cv::Mat &y) {
        cv::Scalar scalarMean = cv::mean(predY - y);
        float locRMSE = cv::sqrt(cv::pow((float)scalarMean[0], 2.f));
        // Log that RMSE
        rmseValidate.emplace_back(locRMSE);
    }


    vector<float> costFunctionPrime(const cv::Mat &x, const cv::Mat &y) {

        cv::Mat predY = forward(x);
        cv::Mat delta;
        bool started = false;

        for (unsigned long i = layers.size() - 1; i > 0; i--) {
            if (!started) {
                cv::Mat activation = layers.at(i).getActivationSigmoidPrime(layers.at(i).z);
                cv::Mat difference = (y - predY);
                cv::multiply(difference, activation, delta);
                delta = -1 * delta;
                started = true;
            } else {
                cv::Mat activation = layers.at(i).getActivationSigmoidPrime(layers.at(i).z);
                cv::Mat transposedW;
                cv::transpose(layers.at(i).w, transposedW);
                delta = delta * transposedW;
                cv::multiply(delta, activation, delta);
            }

            cv::Mat transposedActivation;
            cv::transpose(layers.at(i - 1).a, transposedActivation);
            cv::Mat gradient = transposedActivation * delta;
            layers.at(i - 1).gradientW = gradient;
        }
        return unwrapGradients();
    }

    void addLayer(int size, int nextLayerSize, int seed = -1,
            std::string randomWeightMode = "") {
        layers.emplace_back(Layer(size, nextLayerSize,seed, std::move(randomWeightMode)));
    }

    cv::Mat forward(const cv::Mat &xScaled) {
        layers[0].getForwardOutput(xScaled);
        layers[0].a = xScaled;
        cv::Mat nextZ = layers[0].a * layers[0].w;

        for(int i = 1; i < layers.size(); i++) {
            nextZ = layers[i].getForwardOutput(nextZ);
        }

        cv::Mat predictedY = layers[layers.size()-1].a;
        return predictedY;
    };

    vector<float> unwrap() {
        vector<float> params;
        for (int i = 0; i < layers.size() - 1; i++) {
            for (int j = 0; j < layers[i].w.rows; j++) {
                params.insert(params.end(), layers[i].w.ptr<float>(j), layers[i].w.ptr<float>(j)+layers[i].w.cols);
            }
        }
        return params;
    }

    vector<float> unwrapGradients() {
        vector<float> params;
        for (int i = 0; i < layers.size()-1; i++) {

            for (int j = 0; j < layers[i].gradientW.rows; j++) {
                params.insert(params.end(), layers[i].gradientW.ptr<float>(j),
                        layers[i].gradientW.ptr<float>(j)+layers[i].gradientW.cols);
            }
        }
        return params;
    }

    void wrap(vector<float> params) {

        vector<float>::const_iterator first;
        vector<float>::const_iterator last;
        for (int i = 0; i < layers.size() - 1; i++) {

            if (i == 0) {
                first = params.begin();
                last = params.begin() + layers[i].w.rows * layers[i].w.cols;
            } else {
                first = last;
                last = last + layers[i].w.rows * layers[i].w.cols;
            }

            vector<float> slice(first, last);
            layers[i].w = cv::Mat(slice, true).reshape(0, layers[i].w.rows);
        }
    }

    void minimizeAdamOptimizer(vector<float> grad, const cv::Mat &x, const cv::Mat &y, int numIterations = 15000) {
        float alpha = 0.0001;
        float beta1 = 0.9;
        float beta2 = 0.999;
        float epsilon = 0.00000001;

        vector<float> m0 = vector<float>(grad.size(), 0);
        vector<float> v0 = vector<float>(grad.size(), 0);
        float t = 0.0;

        vector<float> mt = vector<float>(grad.size(), 0);
        vector<float> vt = vector<float>(grad.size(), 0);

        for (int i = 0; i < numIterations; i++) {
            t++;
            vector<float> gradients = costFunctionPrime(x, y);

            // Calc mt
            cv::multiply(beta1, mt, mt);
            vector<float> temp;
            cv::multiply((1 - beta1), gradients, temp);
            cv::add(mt, temp, mt);
            // Calc vt
            cv::multiply(beta2, vt, vt);
            cv::pow(gradients, 2, gradients);
            cv::multiply((1 - beta2), gradients, temp);
            cv::add(vt, temp, vt);
            // Calc mtHat
            vector<float> mtHat;
            cv::divide(mt, (1 - pow(beta1, t)), mtHat);
            // Calc vtHat
            vector<float> vtHat;
            cv::divide(vt, (1 - pow(beta2, t)), vtHat);
            // Calc newParams
            vector<float> params = unwrap();
            vector<float> alphaMtHat;
            vector<float> sqrtVtHat;
            vector<float> sqrtVtHatPlusEpsilon;
            vector<float> divideAlphaMtHatSqrtVtHatPlusEpsilon;

            cv::multiply(alpha, mtHat, alphaMtHat);
            cv::sqrt(vtHat, sqrtVtHat);
            cv::add(sqrtVtHat, epsilon, sqrtVtHatPlusEpsilon);

            vector<float> newParams;
            cv::divide(alphaMtHat, sqrtVtHatPlusEpsilon, divideAlphaMtHatSqrtVtHatPlusEpsilon);
            cv::subtract(params, divideAlphaMtHatSqrtVtHatPlusEpsilon, newParams);

            // Wrap the new weights
            wrap(newParams);

            // Log the cost
            cost.emplace_back(costFunction(x, y));
        };
    }
};

#endif //NEURALNETDEMO_NEURALNET_H
