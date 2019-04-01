//
// Created by Laivins, Josiah on 2019-03-09.
//

#include <gtest/gtest.h>
#include "../src/utils/DatasetCar.h"
#include <opencv2/core/utils/filesystem.hpp>

using namespace cv::utils::fs;

TEST(DatasetCar, DatasetCar_csv_Read_Shuffle_Test) {
    DatasetCar dataset;
    dataset.readCsv();

    cv::String firstFile = dataset[0].filename;
    ASSERT_TRUE(exists(firstFile.c_str()));
    float firstFileY = dataset[0].steeringAngle;
    ASSERT_FLOAT_EQ(firstFileY, 2.815148426500059f);

    dataset.readCsv(-1, true);
    ASSERT_TRUE(exists(dataset[0].filename.c_str()));
    ASSERT_NE(dataset[0].filename, firstFile);
}

TEST(DatasetCar, DatasetCar_csv_Read_Test) {
    DatasetCar dataset;
    dataset.readCsv();

    cv::String firstFile = dataset.filenames.at(0);
    ASSERT_TRUE(exists(firstFile.c_str()));
    std::string::size_type index = firstFile.rfind("0000.jpg");
    ASSERT_NE(index, std::string::npos);
    cv::String lastFile = dataset.filenames.back();
    ASSERT_TRUE(exists(lastFile.c_str()));
    index = lastFile.rfind("1499.jpg");
    ASSERT_NE(index, std::string::npos);

    float firstFileY = dataset.featureValues.at(0);
    ASSERT_FLOAT_EQ(firstFileY, 2.815148426500059f);
    float lastFileY = dataset.featureValues.back();
    ASSERT_FLOAT_EQ(lastFileY, -113.51461556974994f);

    dataset.readCsv(10);
    firstFile = dataset.filenames.at(0);
    ASSERT_TRUE(exists(firstFile.c_str()));
    index = firstFile.rfind("0000.jpg");
    ASSERT_NE(index, std::string::npos);
    lastFile = dataset.filenames.back();
    ASSERT_TRUE(exists(lastFile.c_str()));
    index = lastFile.rfind("1499.jpg");
    ASSERT_EQ(index, std::string::npos);
    lastFileY = dataset.featureValues.back();
    ASSERT_NE(lastFileY, -113.51461556974994f);
}

TEST(DatasetCar, DatasetCar_AccessOperator_Test) {
    DatasetCar dataset;
    dataset.readCsv(10);

    DatasetCarElement datasetCarElement = dataset[0];

    cv::String firstFile = datasetCarElement.filename;
    ASSERT_TRUE(exists(firstFile.c_str()));
    std::string::size_type index = firstFile.rfind("0000.jpg");
    ASSERT_NE(index, std::string::npos);

    float firstFileY = datasetCarElement.steeringAngle;
    ASSERT_FLOAT_EQ(firstFileY, 2.815148426500059f);
}

TEST(DatasetCar, DatasetCar_PathFind_Test) {
    DatasetCar dataset;

    std::string path = dataset.getCurrentPath();
    std::string::size_type index = path.rfind(std::string("thisstringshouldnotexistinanyonessystemlol"), 0);
    ASSERT_EQ(index, std::string::npos);
    // TODO would like to access this via cmake macro
    index = path.rfind(std::string(dataset.projectRoot));
    ASSERT_NE(index, std::string::npos);
}

TEST(DatasetCar, DatasetCar_DataPathFind_Test) {
    DatasetCar dataset;

    std::string path = dataset.getCurrentPath();
    std::string::size_type index = path.rfind(std::string("thisstringshouldnotexistinanyonessystemlol"), 0);
    ASSERT_EQ(index, std::string::npos);
    // TODO would like to access this via cmake macro
    index = path.rfind(dataset.projectRoot);
    ASSERT_NE(index, std::string::npos);
    std::string dataPath = dataset.getDataPath();

    std::string::size_type dataIndex = dataPath.rfind(dataset.projectRoot + "/data");
    ASSERT_NE(dataIndex, std::string::npos);
}

