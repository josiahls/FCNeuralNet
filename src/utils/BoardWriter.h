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

#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/core.hpp>

#define GetCurrentDir getcwd
#endif

using namespace cv::utils::fs;

class BoardWriter {

public:
    BoardWriter(std::string logDir = "./logs") {
        this->getCurrentPath();
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
};

#endif //NEURALNETDEMO_BOARDWRITER_H
