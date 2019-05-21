//
// Created by Laivins, Josiah on 2019-03-16.
//

#include <iostream>
#include <src/utils/DebugHelpers.h>
#include "../nn/NeuralNet.h"
#include "NeuralNetRun.h"
#include "../utils/DatasetCar.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "../utils/BoardWriter.h"
#include "../utils/Logger.h"

namespace nn {

    auto imageDimension = std::make_tuple(32, 32, 3);

    cv::Mat getImage(DatasetCar& dataset, int index, int h=std::get<0>(imageDimension),
            int w=std::get<1>(imageDimension)) {

        cv::Mat image = imread(dataset[index].filename);
        cv::Mat imageNormalized;
        image.convertTo(imageNormalized, CV_32F, 1.0 / 255, 0);
        cv::resize(imageNormalized, imageNormalized, cv::Size(h, w));

        return imageNormalized.reshape(1,1);
    }

    float getNormalY(DatasetCar& dataset, int index) {
        float y = ((dataset[index].steeringAngle - dataset.minFeatureValue) /
                            (dataset.maxFeatureValue - dataset.minFeatureValue));
        if (dataset.maxFeatureValue - dataset.minFeatureValue == 0) {
            cout << "Note, y is being set to 0" << endl;
            y = 0;
        }

        return (float)y;
    }

    cv::Mat getBinnedY(float y, unsigned long bins) {
        // Convert y to a vector dtype
        long closestBin = lround(y * bins) - 1;
        std::vector<float> preHist(bins, 0);
        preHist[closestBin] = 1;
        cv::Mat histogram(1, static_cast<int>(preHist.size()), CV_32F);
        for(int i=0; i<histogram.cols; ++i)
            histogram.at<float>(0, i) = preHist.at(i);

        return histogram;
    }

    cv::Mat getGuassianBinnedY(cv::Mat y, int smoothSize) {
        cv::Mat smoothedY;
        GaussianBlur(y, smoothedY, cv::Size(0, 0), smoothSize, 0);

        return smoothedY;
    }

    double getActualY(cv::Mat predY, DatasetCar dataset, int bins) {

        int predictedLocation = 0;
        // Get the max locations
        double min, max;
        cv::Point predMinLoc, predMaxLoc;

        // Iterate through each row and get the location of the max
        cv::minMaxLoc(predY.row(0), &min, &max, &predMinLoc, &predMaxLoc);
        predictedLocation = predMaxLoc.x;
        double predictedAngle = predictedLocation / float(bins);
        predictedAngle = (predictedAngle * dataset.maxFeatureValue) - dataset.minFeatureValue;

        return predictedAngle;
    }

    float getAccuracy(cv::Mat predY, cv::Mat y, DatasetCar validationDataset, int bins, float tolarance=5) {
        vector<int> predictedLocations;
        vector<int> actualLocations;

        // Get the max locations
        double min, max;
        cv::Point predMinLoc, predMaxLoc;
        cv::Point minLoc, maxLoc;

        // Iterate through each row and get the location of the max
        for(int i = 0; i < predY.rows; i++) {
            cv::minMaxLoc(predY.row(i), &min, &max, &predMinLoc, &predMaxLoc);
            cv::minMaxLoc(y.row(i), &min, &max, &minLoc, &maxLoc);
            predictedLocations.push_back(predMaxLoc.x);
            actualLocations.push_back(maxLoc.x);
        }
        // Convert each to their actual angles
        vector<float> predictedAngles;
        vector<float> actualAngles;
        for (long i = 0; i < predictedLocations.size(); i++) {
            float predictedAngle = predictedLocations.at(i) / float(bins);
            float actualAngle = actualLocations.at(i) / float(bins);
            predictedAngles.push_back(static_cast<float &&>(predictedAngle * (validationDataset.maxFeatureValue - validationDataset.minFeatureValue) + validationDataset.minFeatureValue));
            actualAngles.push_back(static_cast<float &&>(actualAngle * (validationDataset.maxFeatureValue - validationDataset.minFeatureValue) + validationDataset.minFeatureValue));
        }
        // Determine accuracy of angles within a tolerance
        cv::Mat absOfAngles;
        cv::subtract(predictedAngles, actualAngles, absOfAngles);
        absOfAngles = cv::abs(absOfAngles);
        float acc = 0;

        for (int i = 0; i < absOfAngles.cols; i++) {
            float value = absOfAngles.at<float>(0, i);
            if (value < tolarance) acc++;
        }


        return (acc / absOfAngles.cols) * 100;
    }

    int train(int argc, char **argv) {
        printf("Running Neural Net Run");
        // Load dataset
        DatasetCar dataset(-1);
        dataset.readCsv(100);
        // Setup the writer
        BoardWriter w;

        vector<DatasetCar> d = dataset.split(.9);
        DatasetCar trainDataset = d[0];
        DatasetCar validationDataset = d[1];

        // Create neural net
        NeuralNet nn = NeuralNet();
        nn.addLayer(std::get<0>(imageDimension) * std::get<1>(imageDimension) * std::get<2>(imageDimension),
                1, 0, "glorot");
        nn.addLayer(1, 60, 0, "glorot");
        nn.addLayer(60, 1, 0, "glorot");

        // Init the validation variables
        cv::Mat validationX;
        cv::Mat validationY;
        for (int i = 0; i < validationDataset.getSize(); i++) {
            validationX.push_back(getImage(validationDataset, i));
            validationY.push_back(getBinnedY(getNormalY(validationDataset, i), 60));
        }

        std::vector<float> validationAccuracy = vector<float>();
        std::vector<float> trainAccuracy = vector<float>();

        cv::Mat finalY;
        cv::Mat finalPredY;

        // Define epochs
        int epochs = 30;
        for (int epoch = 0; epoch < epochs; epoch++) {
            printf("\nStarting epoch %i\n", epoch);

            cv::Mat y;
            cv::Mat predY;

            int batchSize = 1;
            for (int i = 0; i < trainDataset.getSize(); i += batchSize) {
                cv::Mat batchX;
                cv::Mat batchY;

                printf("\tStarting batch %i\n", i);

                for (int imageLoc = i, slot = 0; imageLoc < i + batchSize and imageLoc < trainDataset.getSize();
                     imageLoc++, slot++) {
                    try {
                        batchX.push_back(getImage(trainDataset, imageLoc));
                        batchY.push_back(getGuassianBinnedY(getBinnedY(getNormalY(trainDataset, imageLoc), 60), 10));
                    } catch(cv::Exception) {
                        printf("Derp something happened");
                    }
                }

                y.push_back(batchY);
                predY.push_back(nn.forward(batchX));
                debug::ImshowMatrixDisplayer(nn.layers[1].w, std::tuple<int, int, int>(0, 0, 0), 5.5, false, false);

                try {
                    nn.train(batchX, batchY, 300, batchSize);
                } catch(cv::Exception()) {
                    printf("Derp something happened");
                }
                debug::ImshowMatrixDisplayer(nn.layers[1].w, std::tuple<int, int, int>(0, 0, 0), 5.5, false, false);

            }

            y.copyTo(finalY);
            predY.copyTo(finalPredY);

            // Eval the RMSE independent of batches
            nn.logBatchRMSE(predY, y);
            cv::Mat validationPredY = nn.forward(validationX);
            nn.logBatchRMSEValidation(validationPredY, validationY);
            validationAccuracy.push_back(getAccuracy(validationPredY, validationY, validationDataset, 60));
            trainAccuracy.push_back(getAccuracy(predY, y, trainDataset, 60));

            w.write("RMSE", nn.rmse.back(), 0);
            w.write("Validation RMSE", nn.rmseValidate.back(), 0);
            w.write("Validation Accuracy Percent", validationAccuracy.back(), 0);
            w.write("Train Accuracy Percent", trainAccuracy.back(), 0);

            printf("Epoch %i RMSE: %f Validation RMSE: %f\n Validation Accuracy is %f percent\n Train Accuracy is %f percent\n",
                    epoch, nn.rmse.back(), nn.rmseValidate.back(), validationAccuracy.back(), trainAccuracy.back());
        }

        // We also want to save the neural net params
        BoardWriter modelWriter("models", "model", false);
        modelWriter.write("params", nn.unwrap(), 0);

        std::printf("Showing output comparison");
        cv::Mat dst;
        cv::Mat dst2;
        cv::normalize(finalPredY, dst, 0, 1, cv::NORM_MINMAX);
        cv::normalize(finalY, dst2, 0, 1, cv::NORM_MINMAX);
        cv::hconcat(dst,dst2,dst);
        cv::imshow("result",dst);
        cv::waitKey(0);

        return 0;
    }

    int run(int argc, char **argv) {
        DatasetCar dataset(-1);
        dataset.readCsv(-1);
        NeuralNet nn = NeuralNet();
        nn.addLayer(std::get<0>(imageDimension) * std::get<1>(imageDimension)* std::get<2>(imageDimension), 1,
                0, "middle");
        nn.addLayer(1, 60, 0, "glorot");
        nn.addLayer(60, 1, 0, "glorot");
        // Get the model params
        std::vector<cv::String> dirs = Logger::getLogDirs("models");
        std::sort(dirs.begin(), dirs.end());
        cv::String mostRecentDir = dirs.back();
        // Get the files in the directory
        std::vector<cv::String> files = Logger::getLogDirs("models", mostRecentDir);
        // Start reading the csv
        std::ifstream csvReader(files[0].c_str());
        std::string currentLine;
        std::vector<float> params;
        for(int k = 0; std::getline(csvReader, currentLine); k++) {
            std::stringstream tempStream(currentLine);
            std::string temp;
            float number;
            // Go through each col
            for (int i = 0; std::getline(tempStream, temp, ','); i++) {
                std::stringstream iss( temp );
                if (i == 0 && k != 0) {
                    while ( iss >> number ) {
                        params.push_back( number );
                    }
                }
            }
        }
        // Wrap the params back into the nn
        nn.wrap(params);


        // Init the validation variables
        cv::Mat validationX;
        cv::Mat validationY;
        for (int i = 0; i < dataset.getSize(); i++) {
            validationX.push_back(getImage(dataset, i));
            validationY.push_back(getBinnedY(getNormalY(dataset, i), 60));
        }
        // Create a writer for the image accuracy results
        BoardWriter modelWriter("logs", "log", true);

        // Collect the accuracy from the forward X
        std::vector<double> validationActualY = vector<double>();
        std::vector<std::string> validationOriginalImage = vector<std::string>();
        cv::Mat validationPredY = nn.forward(validationX);

        std::printf("\nShowing weight output comparison");
        cv::Mat dst;
        cv::normalize(nn.layers[0].w, dst, 0, 1, cv::NORM_MINMAX);
        dst = dst.reshape(std::get<2>(imageDimension), std::get<0>(imageDimension));
        cv::imshow("result",dst);
        cv::waitKey(0);

        for (int i = 0; i < validationPredY.rows; i++) {
            validationActualY.push_back(getActualY(validationPredY.row(i), dataset, 60));
            validationOriginalImage.push_back(dataset[i].filename);
            modelWriter.write("image", validationActualY.back(), validationOriginalImage.back(), 0);
        }

        printf("done");
        return 0;
    }
}