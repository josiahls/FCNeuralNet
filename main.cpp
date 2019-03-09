#include <iostream>
#include "src/nn/NeuralNet.h"
#include "src/utils/DatasetCar.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"

cv::Mat getImage(DatasetCar& dataset, int index) {
    cv::Mat image = imread(dataset[index].filename);
    cv::Mat imageNormalized;
    image.convertTo(imageNormalized, CV_32F, 1.0 / 255, 0);
    cv::resize(imageNormalized, imageNormalized, cv::Size(64, 64));
    return imageNormalized.reshape(1,1);
}

float getNormalY(DatasetCar& dataset, int index) {
    double y = (dataset[index].steeringAngle - dataset.minFeatureValue) /
                      dataset.maxFeatureValue - dataset.minFeatureValue;

    return (float)y;
}

int main() {
    // Load dataset
    DatasetCar dataset;
    dataset.readCsv(10);

    // Create neural net
    NeuralNet nn = NeuralNet();
    nn.addLayer(64*64*3, 10, 0, "middle");
    nn.addLayer(10, 1, 0, "middle");
    nn.addLayer(1, 1, 0, "middle");

    // Define epochs
    int epochs = 20;
    for (int epoch = 0; epoch < epochs; epoch++) {

        int batchSize = 5;
        for (int i = 0; i < dataset.filenames.size(); i += batchSize) {
            cv::Mat x;
            cv::Mat y;

            for (int imageLoc = i, slot = 0; imageLoc < i + batchSize and imageLoc < dataset.filenames.size();
                 imageLoc++, slot++) {
                x.push_back(getImage(dataset, imageLoc));
                y.push_back(getNormalY(dataset, imageLoc));
            }
            nn.train(x, y);
        }
        printf("Epoch %i RMSE %f\n", epoch, nn.rmse.back());
    }
    return 0;
}

