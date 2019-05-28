//
// Created by Laivins, Josiah on 2019-04-01.
//

#ifndef NEURALNETDEMO_LOGGER_H
#define NEURALNETDEMO_LOGGER_H

#include <stdio.h>  /* defines FILENAME_MAX */
// From: https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
#ifdef WINDOWS
#include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
#include <string>
#include <ctime>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/core.hpp>
#include <fstream>

#define GetCurrentDir getcwd
#endif

using namespace cv::utils::fs;

class Logger {

public:
    /**
     * Gets current absolute path to the project.
     *
     * Path is found using OpenCV file io API.
     *
     * @return string representing the absolute path.
     */
    static std::string getCurrentPath();

    /**
     * Gets the full absolute path to the log directory.
     *
     * @param rootLogDirName The directory name of the log directory.
     * @return string representing the absolute path.
     */
    static std::string getLogPath(cv::String rootLogDirName = "logs");

    /**
     * Gets the absolute paths to the individual log folders in the log directory.
     *
     * @param rootLogDirName Name of the log directory.
     * @param subFolder If specified, will return the full path to a specific log folder.
     * @return A vector of all, or a specific path to the log directory.
     */
    static std::vector<cv::String> getLogDirs(cv::String rootLogDirName = "logs", std::string subFolder = "");

    static cv::String projectRootName;
};



#endif //NEURALNETDEMO_LOGGER_H
