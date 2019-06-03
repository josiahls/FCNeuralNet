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


#ifndef FCNEURALNET_LOGGER_H
#define FCNEURALNET_LOGGER_H

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



#endif //FCNEURALNET_LOGGER_H
