#include <iostream>
#include "src/nn/NeuralNet.h"
#include "src/utils/DatasetCar.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
//#include <QtCharts>


cv::Mat getImage(DatasetCar& dataset, int index) {
    cv::Mat image = imread(dataset[index].filename);
    cv::Mat imageNormalized;
    image.convertTo(imageNormalized, CV_32F, 1.0 / 255, 0);
    cv::resize(imageNormalized, imageNormalized, cv::Size(64, 64));
    return imageNormalized.reshape(1,1);
}

float getNormalY(DatasetCar& dataset, int index) {
    double y = (dataset[index].steeringAngle - dataset.minFeatureValue) /
            (dataset.maxFeatureValue - dataset.minFeatureValue);

    return (float)y;
}

cv::Mat getBinnedY(float y, int bins) {
    // Convert y to a vector dtype
    cv::Mat arrayOfArrays = (cv::Mat_<float>(1, 1) << y);
    vector<int> channels = {0};
    vector<int> histSize = {bins};
    vector<float> ranges = {0.f, 1.f};
    cv::Mat histogram;

    cv::calcHist(arrayOfArrays, channels, cv::Mat(), histogram, histSize,ranges);
    cv::transpose(histogram, histogram);

    return histogram;
}

int main() {
    // Load dataset
    DatasetCar dataset(-1);
    dataset.readCsv(-1, true);

    vector<DatasetCar> d = dataset.split(.7);
    DatasetCar trainDataset = d[0];
    DatasetCar validationDataset = d[1];

    // Create neural net
    NeuralNet nn = NeuralNet();
    nn.addLayer(64*64*3, 10, 0, "middle");
    nn.addLayer(10, 60, 0, "middle");
    nn.addLayer(60, 1, 0, "middle");

    // Init the validation variables
    cv::Mat validationX;
    cv::Mat validationY;
    for (int i = 0; i < validationDataset.getSize(); i++) {
        cv::vconcat(getImage(validationDataset, i), validationX);
        cv::vconcat(getBinnedY(getNormalY(validationDataset, i), 60), validationY);
    }

    // Define epochs
    int epochs = 20;
    for (int epoch = 0; epoch < epochs; epoch++) {
        printf("Starting epoch %i\n", epoch);

        cv::Mat y;
        cv::Mat predY;

        int batchSize = 35;
        for (int i = 0; i < trainDataset.getSize(); i += batchSize) {
            cv::Mat batchX;
            cv::Mat batchY;
            printf("\tStarting batch %i\n", i);

            for (int imageLoc = i, slot = 0; imageLoc < i + batchSize and imageLoc < trainDataset.getSize();
                 imageLoc++, slot++) {
                batchX.push_back(getImage(trainDataset, imageLoc));
                batchY.push_back(getBinnedY(getNormalY(trainDataset, imageLoc), 60));
            }

            cv::vconcat(batchY, y);
            cv::vconcat(nn.forward(batchX), predY);

            nn.train(batchX, batchY, 15, batchSize);
        }
        // Eval the RMSE independent of batches
        nn.logBatchRMSE(predY, y);
        nn.logBatchRMSEValidation(nn.forward(validationX), validationY);

        printf("Epoch %i RMSE: %f Validation RMSE: %f\n", epoch, nn.rmse.back(), nn.rmseValidate.back());
    }
    return 0;
}

