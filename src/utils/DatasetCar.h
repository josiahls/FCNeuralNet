//
// Created by Laivins, Josiah on 2019-03-09.
//

#ifndef NEURALNETDEMO_DATASET_H
#define NEURALNETDEMO_DATASET_H

#include <stdio.h>  /* defines FILENAME_MAX */

// From: https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #include <string>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <algorithm>
#include <numeric>
#include <random>

#define GetCurrentDir getcwd
#endif

using namespace cv::utils::fs;

struct DatasetCarElement {
    cv::String filename;
    float steeringAngle;
};

struct DatasetBalanceItem {
    int count;
    float value;
};

class DatasetCar {
public:
    std::string projectRoot;
    std::string csvDir;
    std::string csvName;
    std::vector<cv::String> filenames;
    std::vector<float> featureValues;
    std::vector<unsigned long> indexes;

    float minFeatureValue;
    float maxFeatureValue;

    long readableSize;

    DatasetCar(long readableSize=0) {
        projectRoot = "NeuralNetDemo";
        csvDir = "steering_dataset/training";
        csvName = "steering_angles_sanity.csv";

        this->readableSize = readableSize;
    }

    long getSize() {
        return (filenames.empty()) ? this->readableSize : filenames.size();
    }

    DatasetCarElement operator[](unsigned long index) {
        return DatasetCarElement{
            filenames.at(indexes.at(index)),
            featureValues.at(indexes.at(index)),
        };
    }

    void readCsv(int numRows = -1, bool shuffle = false, int seed = -1, bool balance = false,
            std::string method="duplicate") {
        filenames.clear();
        featureValues.clear();

        cv::String dataDir = getDataPath().c_str();
        cv::String fullCsvPath = join(dataDir, csvDir.c_str());
        fullCsvPath = join(fullCsvPath, csvName.c_str());
        // Verify that the path actually exists
        if (!exists(fullCsvPath)) {
            throw "Where is the csv?";
        }

        // Start reading the csv
        std::ifstream csvReader(fullCsvPath.c_str());
        std::string currentLine;
        for(int k = 0; std::getline(csvReader, currentLine); k++) {

            std::stringstream tempStream(currentLine);
            std::string temp;
            // Go through each col
            for (int i = 0; std::getline(tempStream, temp, ','); i++) {
                if (i == 0) {
                    // Its the filename
                    cv::String prefix = "";
                    for(int j = 0; j < 4 - temp.size(); j++) prefix += "0";
                    cv::String imagesDir = join(dataDir.c_str(), csvDir.c_str());
                    imagesDir = join(imagesDir, "images");
                    cv::String fullImageDir = prefix + temp.c_str() + ".jpg";
                    filenames.emplace_back(join(imagesDir, fullImageDir));
                } else if(i == 1) {
                    // Its the actual Y value(s)
                    featureValues.emplace_back(stof(temp));
                }
            }
            // If we set a limit to the number of rows, then do so
            if (k >= numRows and numRows != -1) {
                break;
            }
        }

        csvReader.close();

        double min, max;
        // Get the min and maxes of the features for normalization
        cv::minMaxLoc(featureValues, &min, &max);
        minFeatureValue = (float) min;
        maxFeatureValue = (float) max;

        // If balancing is requested then:
        if (balance) balanceDataset("duplicate");

        // If specified, shuffle
        indexes.resize(filenames.size());
        std::iota(indexes.begin(), indexes.end(), 0);

        if (shuffle) {
            // Handle random initialization
            std::default_random_engine generator(std::random_device{}());
            // To help with debugging, the seed can be defined. The unit test cases take this and
            // set the seed to 0
            if (seed != -1) generator.seed(static_cast<unsigned int>(seed));

            std::shuffle(indexes.begin(), indexes.end(), generator);
        }
    }

    /**
     * balanceDataset will evaluate the data that has been loaded, and try to make the dataset balanced.
     *
     * This is done via either deletion or duplication.
     *
     * @param method - Can either be "delete" or "duplicate" or "middle". Delete would be the most damaging way
     *                 of improving the dataset due to deleting large numbers of data. "duplicate"
     *                 will duplicate under-represented data which can grow the dataset massively.
     *                 "middle" will try to change the data to the average unique y count.
     */
    void balanceDataset(std::string method = "delete", float tolerance=5) {
        // Get distribution of values
        std::vector<DatasetBalanceItem> counts;

        // Log the counts of items and their values
        for (unsigned long i =0; i < this->featureValues.size(); i++) {
            bool found = false;
            // Is this value in the counts vector?
            for (auto& item : counts) {
                if (std::abs(this->featureValues.at(i) - item.value) < tolerance) {
                    item.count++;
                    found = true;
                    break;
                }
            }
            // If not found then we want to add this to the counts vector
            if (!found) counts.push_back(DatasetBalanceItem{0, this->featureValues.at(i)});
        }

        // If specified, shuffle
        std::vector<unsigned long> preDatasetAugIndexes;
        preDatasetAugIndexes.resize(filenames.size());
        std::iota(preDatasetAugIndexes.begin(), preDatasetAugIndexes.end(), 0);

        // Handle random initialization
        std::default_random_engine generator(std::random_device{}());
        std::shuffle(preDatasetAugIndexes.begin(), preDatasetAugIndexes.end(), generator);

        // Distribute
        int maxCount = std::max_element(counts.begin(), counts.end(),
                [](const DatasetBalanceItem &c1, const DatasetBalanceItem &c2){
            return c1.count < c2.count;
        }).base()->count;


        int totalCount = 0;
        for (auto& item: counts) {
            totalCount += item.count;
        }
        int average = totalCount / counts.size();
        if (method == "middle") maxCount = average;

        std::vector<cv::String> tempFilenames;
        std::vector<float> tempFeatureValues;

        // Go through the counts and duplicate under-represented entries
        for (auto &item : counts) {
            item.count = 0;
            // Go through the random indices
            for (unsigned long i =0; i < preDatasetAugIndexes.size(); i++) {
                long index = preDatasetAugIndexes.at(i);

                // If the item is already at the max, then just continue
                if (item.count == maxCount) break;

                // If the item is already at the max, then just continue
                if (item.count < maxCount && i == preDatasetAugIndexes.size()-1) i = 0;

                if (std::abs(this->featureValues.at(index) - item.value) < tolerance) {
                    tempFeatureValues.push_back(this->featureValues.at(index));
                    tempFilenames.push_back(this->filenames.at(index));
                    item.count++;
                }
            }
//            // If the item is already at the max, then just continue
//            if (item.count == maxCount) continue;
//
//            // Otherwise, we are going to find the features that have the value within that tolerance and duplicate them
//            for (unsigned long i =0; i < preDatasetAugIndexes.size(); i++) {
//                long index = preDatasetAugIndexes.at(i);
//
//                // If the item is already at the max, then just continue
//                if (item.count == maxCount) break;
//
//                // If the item is already at the max, then just continue
//                if (item.count != maxCount && i == preDatasetAugIndexes.size()-1) i = 0;
//
//                // If they are within the tolerance, add them again to the base vector.
//                // Since we shuffled the indexes, the duplicated entries should be random enough to avoid bias.
//                if (std::abs(this->featureValues.at(index) - item.value) < tolerance) {
//                    if (item.count < maxCount && (method == "duplicate" || method == "middle")) {
//                        this->featureValues.push_back(this->featureValues.at(index));
//                        this->filenames.push_back(this->filenames.at(index));
//                        item.count++;
//                    } else if (item.count > maxCount && method == "middle") {
//                        this->featureValues.push_back(this->featureValues.at(index));
//                        this->filenames.push_back(this->filenames.at(index));
//                        item.count--;
//                    }
//                }
//            }
        }
        this->filenames = tempFilenames;
        this->featureValues = tempFeatureValues;

    }

    std::string getCurrentPath(std::string lookingFor = "data") {
        char cCurrentPath[FILENAME_MAX];

        // Verify that getting the current path is possible
        if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
        {
            printf("Unable to acquire current path");
        }

        cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

        if (!cv::utils::fs::exists(cv::String(cCurrentPath))) {
            throw "The current directory cannot be resolved.";
        }

        return std::string(cCurrentPath);
    }

    std::string getDataPath() {
        std::string currentPath = getCurrentPath();
        std::string::size_type index = currentPath.rfind(projectRoot);
        std::string projectPath = currentPath.substr(0, index + projectRoot.size() + 1);
        std::string projectDataPath(projectPath + "data");

        // As a note, the cv::String constructor, maybe based on the version I am using,
        // requires character arrays
        if (!exists(projectDataPath.c_str())) {
            throw "Where is your data folder? It should be in PROJECT_NAME/data.";
        }

        return projectDataPath;
    }

    std::vector<DatasetCar> split(float splitPercent) {
        DatasetCar train;
        DatasetCar validate;

        // Define the local filenames, fileValues
        std::vector<cv::String> localFileNames;
        std::vector<float> localFeatureValues;

        for (auto& index: indexes) {
            localFileNames.push_back(this->operator[](index).filename);
            localFeatureValues.push_back(this->operator[](index).steeringAngle);
        }


        int splitIndex = int(splitPercent * localFileNames.size());
        std::vector<cv::String>::const_iterator firstFileNames = localFileNames.begin();
        std::vector<cv::String>::const_iterator lastFileNames = localFileNames.begin() + splitIndex;
        std::vector<float>::const_iterator firstFeatureValues = localFeatureValues.begin();
        std::vector<float>::const_iterator lastFeatureValues = localFeatureValues.begin() + splitIndex;
        // Setup train
        train.filenames = std::vector<cv::String>(firstFileNames, lastFileNames);
        train.featureValues = std::vector<float>(firstFeatureValues, lastFeatureValues);
        train.indexes.resize(train.filenames.size());
        std::iota(train.indexes.begin(), train.indexes.end(), 0);

        // Setup validate
        firstFileNames = localFileNames.begin() + splitIndex;
        lastFileNames = localFileNames.end();
        firstFeatureValues = localFeatureValues.begin() + splitIndex;
        lastFeatureValues = localFeatureValues.end();

        validate.filenames = std::vector<cv::String>(firstFileNames, lastFileNames);
        validate.featureValues = std::vector<float>(firstFeatureValues, lastFeatureValues);
        validate.indexes.resize(validate.filenames.size());
        std::iota(validate.indexes.begin(), validate.indexes.end(), 0);

        // Set the max and min feature values
        train.maxFeatureValue = this->maxFeatureValue;
        validate.maxFeatureValue = this->maxFeatureValue;
        train.minFeatureValue = this->minFeatureValue;
        validate.minFeatureValue = this->minFeatureValue;

        // Bundle both into a vector
        std::vector<DatasetCar> returningSet = {train, validate};
        return returningSet;
    }
};

#endif //NEURALNETDEMO_DATASET_H
