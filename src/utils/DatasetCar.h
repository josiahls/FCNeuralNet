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


#ifndef FCNEURALNET_DATASETCAR_H
#define FCNEURALNET_DATASETCAR_H

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

    /**
     *
     * Handles dataset loading, access, shuffling, and splitting.
     *
     * As a note, I want to change project root to some cmake implementation, however
     *
     * @param readableSize The amount of data to load from the csv
     * @param csvName The name of the csv to load from
     * @param csvDir The directory the csv is located in
     * @param projectRoot The root to the project
     */
    DatasetCar(long readableSize=0, std::string csvName="steering_angles.csv",
            std::string csvDir="steering_dataset/training", std::string projectRoot="FCNeuralNet");

    /**
     * Returns the number of samples loaded.
     *
     * @return A long representing the number of samples
     */
    long getSize();

    /**
     * Overided operator [] for accessing items from the dataset object.
     *
     * @param index The index / sample location we would like to access.
     * @return A DatasetCarElement
     */
    DatasetCarElement operator[](unsigned long index) {
        return DatasetCarElement{
            filenames.at(indexes.at(index)),
            featureValues.at(indexes.at(index)),
        };
    }

    /**
     * Complex method for handling the csv read procedures.
     * Can specify the number of rows from the read samples we want to actually use.
     *
     * @param numRows Number of rows to use. The reason this is here, and the read size must be specified in the
     * constructor is because we might want to load all rows, shuffle them, then only use a portion of them.
     * @param shuffle True or False if shuffling should be done.
     * @param seed Seed for shuffling.
     * @param balance True or False if we want to balance the dataset (remove bias).
     * @param method All methods look at the 'y' values and try to modify the sample set to create an equal representation of y values.
     * This can be done by duplicating under-represented samples, deleting over represented samples, or doing a middle delete / duplicate.
     */
    void readCsv(int numRows = -1, bool shuffle = false, int seed = -1, bool balance = false,
            std::string method="duplicate");

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
    void balanceDataset(std::string method = "delete", float tolerance=5);

    /**
     * Gets the current path of project directory.
     *
     * @return A absolute path as a string to the directory.
     */
    std::string getCurrentPath();

    /**
     * Gets the absolute path to the data directory.
     * 
     * @return A absolute path as a string to the data directory.
     */
    std::string getDataPath();

    /**
     * Will output a vector of 2 DatasetCar objects that have been split.
     * 
     * The intention here is to easily create a training and validation set.
     * 
     * @param splitPercent Proportion of the samples to be assigned to each dataset.
     * @return A vector of 2 DatasetCar objects.
     */
    std::vector<DatasetCar> split(float splitPercent);
};

#endif //FCNEURALNET_DATASETCAR_H
