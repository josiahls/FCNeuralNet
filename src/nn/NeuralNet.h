
#ifndef NEURALNETDEMO_NEURALNET_H
#define NEURALNETDEMO_NEURALNET_H

#include <utility>
#include <iostream>
#include <vector>
#include "../layers/Layer.h"
#include "../utils/DebugHelpers.h"
#include <opencv2/core/mat.hpp>
#include <opencv/cxmisc.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"


using namespace std;


class NeuralNet {
public:
    vector<Layer> layers;
    vector<float> rmse;
    vector<float> rmseValidate;
    vector<float> cost;

    /**
     * Initializes a neural net instance. Next step is calling the add layer methods for the creatign the net.
     */
    NeuralNet();;

    /**
     * Based on an input matrix N x D where N is the number of samples, and D is the number of features,
     * output the predicted y values. As a note, the input needs to be normalized, and the output needs to be
     * unnormlized.
     *
     * @param x The input matrix composed of samples N x D where N is the number of samples, and D is the number
     * of features
     * @return A matrix of predicted y values.
     */
    cv::Mat predict(const cv::Mat &x);;

    /**
     * Uses the formula sum((1/2) * (y - y_hat)^2) for outputting the total "incorrectness" of the neural net.
     *
     * @param x An input x matrix composed of samples N x D where N is the number of samples, and D is the number
     * of features
     * @param y Actual y values composed of samples N x K where N is the number of samples, and K is the number
     * of features
     * @return A single double scalar representing the cost J
     */
    double costFunction(const cv::Mat &x, const cv::Mat &y);

    /**
     * Do a single epoch of training on a given x and y samples.
     *
     * @param x An input x matrix composed of samples N x D where N is the number of samples, and D is the number
     * of features
     * @param y Actual y values composed of samples N x K where N is the number of samples, and K is the number
     * of features
     * @param numIterations Number of iterations for the optimizer to follow
     * @param batchSize Currently not used. This is being taken care of by the executing code.
     */
    void train(const cv::Mat &x, const cv::Mat &y, int numIterations = 15000, int batchSize = -1) {
        vector<float> paramsInitial = unwrap();
        vector<float> unwrappedV = costFunctionPrime(x, y);

        minimizeAdamOptimizer(unwrappedV, x, y, numIterations);

        if (batchSize == -1) {
            logRMSE(x, y);
        }
    }

    /**
     * Logs the RMSE of the given x and y samples, and saves them into an internal vector tracked by the neural net
     * instance.
     *
     * @param x An input x matrix composed of samples N x D where N is the number of samples, and D is the number
     * of features
     * @param y Actual y values composed of samples N x K where N is the number of samples, and K is the number
     * of features
     */
    void logRMSE(const cv::Mat &x, const cv::Mat &y) {
        cv::Mat predY = forward(x);
        // Calc the RMSE
        cv::Scalar scalarMean = cv::mean(predY - y);
        float locRMSE = cv::sqrt(cv::pow((float)scalarMean[0], 2.f));
        // Log that RMSE
        rmse.emplace_back(locRMSE);
    }

    /**
     * Different from logRMSE in that this is to be executed after running through all of the batches.
     * The goal here is to log a more accurate represention of the error the model produces.
     *
     * @param predY The predicted y values.
     * @param y Actual y values composed of samples N x K where N is the number of samples, and K is the number
     * of features
     */
    void logBatchRMSE(const cv::Mat &predY, const cv::Mat &y);

    /**
     * Different from logRMSE in that this is to be executed after running through all of the batches.
     * Main difference between this and logBatchRMSE is that this validation RMSE will be saved in a rmseValidate field.
     * The goal here is to log a more accurate represention of the error the model produces.
     *
     * @param predY The predicted y values.
     * @param y Actual y values composed of samples N x K where N is the number of samples, and K is the number
     * of features
     */
    void logBatchRMSEValidation(const cv::Mat &predY, const cv::Mat &y);

    /**
     * Outputs the gradients of all the parameters in the neural net given an x and y sample input.
     *
     * @param x An input x matrix composed of samples N x D where N is the number of samples, and D is the number
     * of features
     * @param y Actual y values composed of samples N x K where N is the number of samples, and K is the number
     * of features
     * @return A vector representing the gradients of all the parameters in all the layers. Used for optimizers.
     */
    vector<float> costFunctionPrime(const cv::Mat &x, const cv::Mat &y);

    /**
     * Handles adding a layer to the neural net.
     *
     * @param size Number of neurons in the layer
     * @param nextLayerSize The number of neuron in the next layer.
     * @param seed The random seed for weight initialization.
     * @param randomWeightMode The randomization method.
     * @param activation The activation type. Currently either "tanh" or "sigmoid"
     */
    void addLayer(int size, int nextLayerSize, int seed = -1,
            std::string randomWeightMode = "", std::string activation = "");

    /**
     * Pushes an x input through the neural net, and outputs the predicted y value.
     *
     * @param xScaled An input x matrix composed of samples N x D where N is the number of samples, and D is the number
     * of features
     * @return Predicted y values composed of samples N x K where N is the number of samples, and K is the number
     * of features
     */
    cv::Mat forward(const cv::Mat &xScaled);;

    /**
     * Unwraps the neural net's parameters into a single floating point vector.
     *
     * @param start Which layer to start unwrapping the parameters.
     * @return Unwrapped parameters of the neural net. These represent the weights of the layers.
     */
    vector<float> unwrap(int start=0);

    /**
     * Unwraps the gradients of the parameters in the neural net into a single floating point vector.
     *
     * @return Unwrapped parameter gradients of the neural net. These represent how much each parameter is supposed to
     * change.
     */
    vector<float> unwrapGradients();

    /**
     * Wraps a vector of parameters into the neural net. It is important for the neural net to have the same shape
     * as what the vectors are expecting.
     *
     * @param params A vector of floating point values representing the weights of the neurons of the layers in the
     * neural net.
     */
    void wrap(vector<float> params) {

        vector<float>::const_iterator first;
        vector<float>::const_iterator last;
        vector<float>  m1;
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
            m1 = debug::unwrapMat(layers[i].w);

        }
    }

    /**
     * The Adam optimizer. Found from https://github.com/sagarvegad/Adam-optimizer/blob/master/Adam.py
     *
     * @param grad The parameter gradients.
     * @param x An input x matrix composed of samples N x D where N is the number of samples, and D is the number
     * of features
     * @param y Actual y values composed of samples N x K where N is the number of samples, and K is the number
     * of features
     * @param numIterations Number of iterations. Be careful, we don't want to over optimize if doing batch training.
     */
    void minimizeAdamOptimizer(vector<float> grad, const cv::Mat &x, const cv::Mat &y, int numIterations = 15000);
};

#endif //NEURALNETDEMO_NEURALNET_H
