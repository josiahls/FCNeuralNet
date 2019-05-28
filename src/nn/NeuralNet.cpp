//
// Created by Laivins, Josiah on 2019-03-05.
//

#include "NeuralNet.h"

NeuralNet::NeuralNet() {
    this->layers = vector<Layer>();
    this->rmse = vector<float>();
    this->rmseValidate = vector<float>();
    this->cost = vector<float>();
}

cv::Mat NeuralNet::predict(const cv::Mat &x) {
    cv::Mat y = forward(x);
    return y;
}

double NeuralNet::costFunction(const cv::Mat &x, const cv::Mat &y) {
    // FORMULA 3: J = sum((1/2) * (y - y_hat)^2)
    cv::Mat predY = forward(x);

    cv::Mat subtractionToPower;
    cv::pow(predY - y, 2, subtractionToPower);
    std::vector<float> prediction = debug::unwrapMat(predY);
    std::vector<float> acty = debug::unwrapMat(y);
    std::vector<float> subtopow = debug::unwrapMat(subtractionToPower);

    float summation = static_cast<float>(cv::sum(subtractionToPower)[0]) * 0.5f;
    return summation;
}

void NeuralNet::logBatchRMSE(const cv::Mat &predY, const cv::Mat &y) {
    cv::Scalar scalarMean = cv::mean(predY - y);
    float locRMSE = cv::sqrt(cv::pow((float)scalarMean[0], 2.f));
    // Log that RMSE
    rmse.emplace_back(locRMSE);
}

void NeuralNet::logBatchRMSEValidation(const cv::Mat &predY, const cv::Mat &y) {

    cv::Mat subMat;
    cv::subtract(predY, y, subMat);
    cv::Scalar scalarMean = cv::mean(subMat);
    float locRMSE = cv::sqrt(cv::pow((float)scalarMean[0], 2.f));
    // Log that RMSE
    rmseValidate.emplace_back(locRMSE);
}

vector<float> NeuralNet::costFunctionPrime(const cv::Mat &x, const cv::Mat &y) {

    cv::Mat predY = forward(x);
    std::vector<float> prediction = debug::unwrapMat(predY);
    std::vector<float> actual = debug::unwrapMat(y);
    cv::Mat delta;
    bool started = false;

    for (unsigned long i = layers.size() - 1; i > 0; i--) {
        if (!started) {
            cv::Mat activation = layers.at(i).getActivationPrime(layers.at(i).z);
            cv::Mat difference;
            cv::subtract(y, predY, difference);
            std::vector<float> diff = debug::unwrapMat(difference);
            std::vector<float> act = debug::unwrapMat(activation);
            cv::multiply(difference, activation, delta);
            delta = -1 * delta;
            started = true;


        } else {
            cv::Mat activation = layers.at(i).getActivationPrime(layers.at(i).z);
            cv::Mat transposedW;
            cv::transpose(layers.at(i).w, transposedW);
            std::vector<float> del = debug::unwrapMat(delta);
            delta = delta * transposedW;

            cv::multiply(delta, activation, delta);
        }

        std::vector<float> del = debug::unwrapMat(delta);
        cv::Mat transposedActivation;
        cv::transpose(layers.at(i - 1).a, transposedActivation);
        std::vector<float> ta = debug::unwrapMat(transposedActivation);
        cv::Mat gradient = transposedActivation * delta; // Buggy multiplication (kills negatives?)
//            cv::Mat gradient;
//            cv::multiply(transposedActivation, delta, gradient);
        std::vector<float> grad = debug::unwrapMat(gradient);
        layers.at(i - 1).gradientW = gradient;
        std::vector<float> gradw = debug::unwrapMat(gradient);
    }

    return unwrapGradients();
}

void NeuralNet::addLayer(int size, int nextLayerSize, int seed, string randomWeightMode, string activation) {
    layers.emplace_back(Layer(size, nextLayerSize,seed, std::move(randomWeightMode), activation));
}

cv::Mat NeuralNet::forward(const cv::Mat &xScaled) {
    layers[0].getForwardOutput(xScaled);
    layers[0].a = xScaled;

    cv::Mat testA = layers[0].a;
    cv::Mat testW = layers[0].w;
    cv::Mat nextZ = layers[0].a * layers[0].w;

    for(int i = 1; i < layers.size(); i++) {
        nextZ = layers[i].getForwardOutput(nextZ);
    }

    cv::Mat predictedY = layers[layers.size()-1].a;
    std::vector<float> pry = debug::unwrapMat(predictedY);
    return predictedY;
}

vector<float> NeuralNet::unwrap(int start) {
    vector<float> params;
    for (int i = start; i < layers.size() - 1; i++) {
        for (int j = 0; j < layers[i].w.rows; j++) {
            params.insert(params.end(), layers[i].w.ptr<float>(j), layers[i].w.ptr<float>(j)+layers[i].w.cols);
        }
    }
    return params;
}

vector<float> NeuralNet::unwrapGradients() {
    vector<float> params;
    for (int i = 0; i < layers.size()-1; i++) {

        for (int j = 0; j < layers[i].gradientW.rows; j++) {
            params.insert(params.end(), layers[i].gradientW.ptr<float>(j),
                          layers[i].gradientW.ptr<float>(j)+layers[i].gradientW.cols);
        }
    }
    return params;
}

void NeuralNet::minimizeAdamOptimizer(vector<float> grad, const cv::Mat &x, const cv::Mat &y, int numIterations) {
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
//            debug::ImshowMatrixDisplayer(this->layers[0].w, std::tuple<int, int, int>(0, 0, 900), 5.5, false, true);
//            debug::unwrapMat(this->layers[0].w);
//            debug::unwrapMat(this->layers[1].w);
        cv::divide(alphaMtHat, sqrtVtHatPlusEpsilon, divideAlphaMtHatSqrtVtHatPlusEpsilon);
        cv::subtract(params, divideAlphaMtHatSqrtVtHatPlusEpsilon, newParams);
//            debug::ImshowMatrixDisplayer(newParams, std::tuple<int, int, int>(0, 0, 900), 5.5, false, true);
        // Wrap the new weights
        wrap(newParams);

        // Log the cost
        cost.emplace_back(costFunction(x, y));
    };
}
