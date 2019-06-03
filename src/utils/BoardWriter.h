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


#ifndef FCNEURALNET_BOARDWRITER_H
#define FCNEURALNET_BOARDWRITER_H

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

    /**
     * The BoardWriter handles logging data into a csv file for visualizing by another application.
     *
     * @param logDir Name of the directory to save log directories
     * @param logRootName Root name of a single log directory. Dates and extra info will be appended to this.
     * @param useExisting If True will load an existing most recent log directory and continue appending data to it.
     */
    explicit BoardWriter(std::string logDir = "logs", std::string logRootName = "log", bool useExisting = false);

    /**
     * If useExisting in the constructor has been set to true, then we will load the existing most recent log
     * to append data to.
     *
     */
    void initExistingLog();

    /**
     * Will initialize a new log directory for writing data to.
     *
     */
    void initLog();

    /**
     * Will write a single log value with a tag.
     * Handles adding new sections to an existing csv if the tag does not exist,
     * and appending data based on an existing tag.
     *
     *
     * @param name Will serve as the unique tag name, and entry name
     * @param value The value to write
     * @param flag Currently not implemented.
     */
    void write(std::string name, float value, int flag);

    /**
     * Will write a list of log values with a tag.
     * Handles adding new sections to an existing csv if the tag does not exist,
     * and appending data based on an existing tag.
     *
     *
     * @param name Will serve as the unique tag name, and entry name
     * @param value The values to write.
     * @param flag Currently not implemented.
     */
    void write(std::string name, std::vector<float> value, int flag);

    /**
     * Will write a value with a tag.
     * Handles adding new sections to an existing csv if the tag does not exist,
     * and appending data based on an existing tag.
     *
     * This method is used primarly for logging values, and some file.
     * For example, steering angles given a specific image file.
     *
     *
     * @param name Will serve as the unique tag name, and entry name
     * @param value The values to write.
     * @param filename Name of a file the value is associated with.
     * @param flag Currently not implemented.
     */
    void write(std::string name, double value, std::string filename, int flag);

    /**
     * Gets current absolute path to the project.
     *
     * Path is found using OpenCV file io API.
     *
     * @return string representing the absolute path.
     */
    std::string getCurrentPath();

    /**
     * Gets the full absolute path to the log directory.

     * @return string representing the absolute path.
     */
    std::string getLogPath();

    /**
     * Gets the absolute paths to the individual log folders in the log directory.
     *
     * @param subFolder Name of a specific sub directory.
     * @return A vector of all, or a specific path to the log directory.
     */
    std::vector<cv::String> getLogDirs(std::string subFolder = "");

    /**
     * Utility method for unit testing. Allows for clean test log deletion.
     */
    void removeSubDir();
};

#endif //FCNEURALNET_BOARDWRITER_H
