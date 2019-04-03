//
// Created by Laivins, Josiah on 2019-04-01.
//

#ifndef NEURALNETDEMO_HELPERS_H
#define NEURALNETDEMO_HELPERS_H

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
    static std::string getCurrentPath(){
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

    static std::string getLogPath(cv::String rootLogDirName = "logs") {
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

    static std::vector<cv::String> getLogDirs(cv::String rootLogDirName = "logs", std::string subFolder = "") {
        cv::String folder = rootLogDirName;
        if (rootLogDirName.find('/') == std::string::npos) {
            folder = Logger::getLogPath(rootLogDirName);
        }

        if (subFolder != "") {
            if (subFolder.find('/') == std::string::npos || subFolder.find('\\') == std::string::npos) {
                folder = subFolder;
            } else {
                folder = join(folder, subFolder);
            }
        }

        std::vector<cv::String> filenames;

        glob(folder, "", filenames, false, true); // new function that does the job ;-)
        return filenames;
    }

    static cv::String projectRootName;
};



#endif //NEURALNETDEMO_HELPERS_H
