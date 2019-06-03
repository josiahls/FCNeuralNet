/**
 * FCNeuralNet
 * Created by Laivins, Josiah https://josiahls.github.io/ on 2019-03-08
 *
 * This software is provided 'as-is', without any express or implied warranty.
 *
 * In no event will the author(s) be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */


#include <iostream>
#include "../utils/DebugHelpers.h"
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

    auto imageDimension = std::make_tuple(32, 32, 1);

    cv::Mat getImage(DatasetCar& dataset, int index, int h=std::get<0>(imageDimension),
            int w=std::get<1>(imageDimension)) {

        cv::Mat image = imread(dataset[index].filename);
        cv::Mat imageNormalized;
        image.convertTo(imageNormalized, CV_32F, 1.0 / 255, 0);
        cv::resize(imageNormalized, imageNormalized, cv::Size(h, w));
        cv::cvtColor(imageNormalized, imageNormalized, CV_BGR2GRAY);
        cv::Mat normedImage = imageNormalized.reshape(1,1);

        return normedImage;
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
        predictedAngle = (predictedAngle * (dataset.maxFeatureValue - dataset.minFeatureValue) + dataset.minFeatureValue);

        return predictedAngle;
    }

    float getAccuracyFromBinned(cv::Mat predY, cv::Mat y, DatasetCar validationDataset, int bins, float tolarance = 5) {
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

    float getAccuracyFromContinuous(cv::Mat predY, cv::Mat y, DatasetCar validationDataset, float tolarance = 5) {
        vector<float> predictedLocations;
        vector<float> actualLocations;

        // Iterate through each row and get the location of the max
        for(int i = 0; i < predY.rows; i++) {
            predictedLocations.push_back(predY.at<float>(i, 0));
            actualLocations.push_back(y.at<float>(i, 0));
        }
        // Convert each to their actual angles
        vector<float> predictedAngles;
        vector<float> actualAngles;
        for (long i = 0; i < predictedLocations.size(); i++) {
            float predictedAngle = predictedLocations.at(i);
            float actualAngle = actualLocations.at(i);
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
        DatasetCar dataset(-1, "steering_angles.csv");
        dataset.readCsv(-1, true, -1, true);
        // Setup the writer
        BoardWriter w;

        vector<DatasetCar> d = dataset.split(.9);
        DatasetCar trainDataset = d[0];
        DatasetCar validationDataset = d[1];

        // Create neural net
        NeuralNet nn = NeuralNet();
        nn.addLayer(std::get<0>(imageDimension) * std::get<1>(imageDimension) * std::get<2>(imageDimension),
                60, 0, "glorot", "tanh");
        nn.addLayer(60, 1, 0, "glorot", "tanh");
//        nn.addLayer(60, 1, 0, "glorot", "tanh");
        printf("Reading stuff");
        // Init the validation variables
        cv::Mat validationX;
        cv::Mat validationY;
        for (int i = 0; i < validationDataset.getSize(); i++) {
            validationX.push_back(getImage(validationDataset, i));
            validationY.push_back(getBinnedY(getNormalY(validationDataset, i), 60));
//            validationY.push_back(getNormalY(validationDataset, i));
        }

        std::vector<float> validationAccuracy = vector<float>();
        std::vector<float> trainAccuracy = vector<float>();

        cv::Mat finalY;
        cv::Mat finalPredY;

        // Define epochs
        int epochs = 10;
        for (int epoch = 0; epoch < epochs; epoch++) {
            printf("\nStarting epoch %i\n", epoch);

            cv::Mat y;
            cv::Mat predY;

            int batchSize = 2;
            for (int i = 0; i < trainDataset.getSize(); i += batchSize) {
                cv::Mat batchX;
                cv::Mat batchY;

                printf("\tStarting batch %i\n", i);

                for (int imageLoc = i, slot = 0; imageLoc < i + batchSize and imageLoc < trainDataset.getSize();
                     imageLoc++, slot++) {
                    try {
                        batchX.push_back(getImage(trainDataset, imageLoc));
                        batchY.push_back(getGuassianBinnedY(getBinnedY(getNormalY(trainDataset, imageLoc), 60), 3));
                    } catch(cv::Exception) {
                        printf("Derp something happened");
                    }
                }
                y.push_back(batchY);
                predY.push_back(nn.forward(batchX));

                try {
                    nn.train(batchX, batchY, 3 * batchSize, batchSize);
                } catch(cv::Exception()) {
                    printf("Derp something happened");
                }

                std::vector<float> prediction = debug::unwrapMat(nn.predict(batchX));
                std::vector<float> actual = debug::unwrapMat(batchY);

            }

            y.copyTo(finalY);
            predY.copyTo(finalPredY);

            // Eval the RMSE independent of batches
            nn.logBatchRMSE(predY, y);
            cv::Mat validationPredY = nn.forward(validationX);
            nn.logBatchRMSEValidation(validationPredY, validationY);
            validationAccuracy.push_back(getAccuracyFromBinned(validationPredY, validationY, validationDataset, 60));
            trainAccuracy.push_back(getAccuracyFromBinned(predY, y, trainDataset, 60));

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
//        debug::ImshowMatrixDisplayer(dst, std::tuple<int, int, int>(0, 0, 0), 2, true, false);

        return 0;
    }

    int run(int argc, char **argv) {
        DatasetCar dataset(-1, "steering_angles.csv");
        dataset.readCsv(-1);
        NeuralNet nn = NeuralNet();
        nn.addLayer(std::get<0>(imageDimension) * std::get<1>(imageDimension)* std::get<2>(imageDimension), 60,
                0, "glorot");
        nn.addLayer(60, 1, 0, "glorot");
//        nn.addLayer(60, 1, 0, "glorot");
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

        std::vector<float> predy = debug::unwrapMat(validationPredY);

//        std::printf("\nShowing weight output comparison");
//        cv::Mat dst;
//        cv::normalize(nn.layers[0].w, dst, 0, 1, cv::NORM_MINMAX);
//        dst = dst.reshape(std::get<2>(imageDimension), std::get<0>(imageDimension));
//        cv::imshow("result",dst);
//        cv::waitKey(0);

        for (int i = 0; i < validationPredY.rows; i++) {
            validationActualY.push_back(getActualY(validationPredY.row(i), dataset, 60));
            validationOriginalImage.push_back(dataset[i].filename);
            modelWriter.write("image", validationActualY.back(), validationOriginalImage.back(), 0);
        }

//        std::printf("Showing output comparison");
//        cv::Mat dst;
//        cv::Mat dst2;
//        cv::normalize(validationPredY, dst, 0, 1, cv::NORM_MINMAX);
//        cv::normalize(validationY, dst2, 0, 1, cv::NORM_MINMAX);
//        cv::hconcat(dst,dst2,dst);
//        debug::ImshowMatrixDisplayer(dst, std::tuple<int, int, int>(0, 0, 0), 2, true, false);

        printf("done");
        return 0;
    }
}