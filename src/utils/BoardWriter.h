
//
// Created by Laivins, Josiah on 2019-03-15.
//

#ifndef NEURALNETDEMO_BOARDWRITER_H
#define NEURALNETDEMO_BOARDWRITER_H

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

class BoardWriter {

public:
    enum SummaryDTypes {
        ONE_D_VECTOR = 0,
    };

    static std::string projectRootName;
    static std::string logRootName;
    static std::string rootLogDirName;
    std::string logDirRootPath;
    std::string subLogDirPath;
    std::string logBaseFilePath;

    BoardWriter(std::string logDir = "logs", std::string logRootName = "log") {
        BoardWriter::projectRootName = "NeuralNetDemo";
        BoardWriter::logRootName = std::move(logRootName);
        BoardWriter::rootLogDirName = std::move(logDir);
        this->logDirRootPath = this->getLogPath();
        this->subLogDirPath = "";
        this->logBaseFilePath = "";
        this->initLog();
    }

    void initLog() {
        // current date/time based on current system
        time_t currentTime = time(0);
        struct tm * sometime;
        sometime = localtime(&currentTime);

        // construct directory name
        std::string directoryName;
        directoryName += std::to_string(sometime->tm_mday);
        directoryName += std::to_string(sometime->tm_mon + 1);
        directoryName += std::to_string(sometime->tm_year % 100);
        directoryName += "_" + std::to_string(sometime->tm_hour);
        directoryName += std::to_string(sometime->tm_min);
        directoryName += std::to_string(sometime->tm_sec);
        directoryName += "_" + logRootName;

        // Create the directory if it does not exist
        cv::String fullPath = join(logDirRootPath, directoryName.c_str());
        if (exists(fullPath)) {
            printf("The directory already exists. Adding postfixes");
            for(int i = 0; exists(fullPath); i++) {
                directoryName += "v" + std::to_string(i);
                fullPath = join(logDirRootPath, directoryName.c_str());

                if (i > 10) throw std::runtime_error("I tried adding a prefix to the directory to "
                                                     "make it unique and failed.");
            }
        } else {
            // As a note, the cv::String constructor, maybe based on the version I am using,
            // requires character arrays
            printf("Missing log directory. Making.");
            cv::utils::fs::createDirectory(fullPath);
        }

        // Finally, create an empty log file to write to
        cv::String logRootCSVName = cv::String(directoryName);

        this->subLogDirPath = fullPath;
        this->logBaseFilePath = join(fullPath, logRootCSVName);
    }

    void write(std::string name, float value, int flag) {

        cv::String directLogName = logBaseFilePath + "_" + name + ".csv";

        // Verify that the path actually exists
        if (!exists(directLogName)) {
            std::ofstream csvWriter(directLogName);
            csvWriter << name << "," << "dt" << "\n";
            csvWriter.close();

        }

        // Start write new data to the csv
        std::ofstream csvWriter(directLogName, std::ofstream::app);
        csvWriter << value << "," << std::to_string(time(0)) << "\n";
        csvWriter.close();
    }

    std::string getCurrentPath() {
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

    std::string getLogPath() {
        std::string currentPath = getCurrentPath();
        std::string::size_type index = currentPath.rfind(projectRootName);
        std::string projectPath = currentPath.substr(0, index + projectRootName.size() + 1);
        std::string projectLogPath(projectPath + rootLogDirName);

        // As a note, the cv::String constructor, maybe based on the version I am using,
        // requires character arrays
        if (!exists(projectLogPath.c_str())) {
            printf("Missing log directory. Making.");
            cv::utils::fs::createDirectory(projectPath + rootLogDirName);
        }

        return projectLogPath;
    }

    std::vector<cv::String> getLogDirs(std::string subFolder = "") {
        cv::String folder = BoardWriter::getLogPath();
        if (subFolder == "") {
            folder = join(folder, subFolder);
        }

        std::vector<cv::String> filenames;

        glob(folder, filenames); // new function that does the job ;-)
        return filenames;
    }

    std::vector<cv::String> getLogDirAsPaths(std::string subFolder = "") {
        cv::String folder = BoardWriter::getLogPath();
        std::vector<cv::String> filenames = BoardWriter::getLogDirs(std::move(subFolder));
        for (int i = 0; i < filenames.size(); i++) {
            filenames[i] = folder + filenames[i];
        }
        return filenames;
    }

    void removeSubDir() {
        remove_all(subLogDirPath);
    }
};

#endif //NEURALNETDEMO_BOARDWRITER_H
