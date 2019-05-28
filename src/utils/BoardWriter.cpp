//
// Created by Laivins, Josiah on 2019-03-19.
//

#include "BoardWriter.h"

std::string BoardWriter::projectRootName;
std::string BoardWriter::logRootName;
std::string BoardWriter::rootLogDirName;

BoardWriter::BoardWriter(std::string logDir, std::string logRootName, bool useExisting) {
    BoardWriter::projectRootName = "NeuralNetDemo";
    BoardWriter::logRootName = std::move(logRootName);
    BoardWriter::rootLogDirName = std::move(logDir);
    this->logDirRootPath = this->getLogPath();
    this->subLogDirPath = "";
    this->logBaseFilePath = "";

    if (!useExisting) this->initLog();
    // If we want to use an existing log then calling this will override some of the existing params
    if (useExisting) this->initExistingLog();
}

void BoardWriter::initExistingLog() {
    // Get the most recent directory, and init the the number of graphs in that directory
    std::vector<cv::String> dirs = getLogDirs();
    std::sort(dirs.begin(), dirs.end());
    cv::String mostRecentDir = dirs.back();
    // Extract the base filename
    // Get the files in the directory
    std::vector<cv::String> files = getLogDirs(mostRecentDir);


    this->subLogDirPath = mostRecentDir;
    size_t loc = mostRecentDir.rfind('/');
    this->logBaseFilePath = join(mostRecentDir, mostRecentDir.substr(loc+1));
}

void BoardWriter::initLog() {
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
        printf("\nMissing log directory. Making.");
        cv::utils::fs::createDirectory(fullPath);
    }

    // Finally, create an empty log file to write to
    cv::String logRootCSVName = cv::String(directoryName);

    this->subLogDirPath = fullPath;
    this->logBaseFilePath = join(fullPath, logRootCSVName);
}

void BoardWriter::write(std::string name, float value, int flag) {

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

void BoardWriter::write(std::string name, std::vector<float> value, int flag) {

    cv::String directLogName = logBaseFilePath + "_" + name + ".csv";

    // Verify that the path actually exists
    if (!exists(directLogName)) {
        std::ofstream csvWriter(directLogName);
        csvWriter << name << "," << "dt" << "\n";
        csvWriter.close();
    }

    // Start write new data to the csv
    std::ofstream csvWriter(directLogName, std::ofstream::app);
    std::copy(value.begin(), value.end(), std::ostream_iterator<float>(csvWriter, " "));
    csvWriter << "," << std::to_string(time(0)) << "\n";
    csvWriter.close();
}

void BoardWriter::write(std::string name, double value, std::string filename, int flag) {

    cv::String directLogName = logBaseFilePath + "_" + name + ".csv";

    // Verify that the path actually exists
    if (!exists(directLogName)) {
        std::ofstream csvWriter(directLogName);
        csvWriter << name << "," << "filename" << "," << "dt" << "\n";
        csvWriter.close();

    }

    // Start write new data to the csv
    std::ofstream csvWriter(directLogName, std::ofstream::app);
    csvWriter << value << "," << filename << "," << std::to_string(time(0)) << "\n";
    csvWriter.close();
}

std::string BoardWriter::getCurrentPath() {
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

std::string BoardWriter::getLogPath() {
    std::string currentPath = getCurrentPath();
    std::string::size_type index = currentPath.rfind(projectRootName);
    std::string projectPath = currentPath.substr(0, index + projectRootName.size() + 1);
    std::string projectLogPath(projectPath + rootLogDirName);

    // As a note, the cv::String constructor, maybe based on the version I am using,
    // requires character arrays
    if (!exists(projectLogPath.c_str())) {
        printf("\nMissing log directory. Making.");
        cv::utils::fs::createDirectory(projectPath + rootLogDirName);
    }

    return projectLogPath;
}

std::vector<cv::String> BoardWriter::getLogDirs(std::string subFolder) {
    cv::String folder = BoardWriter::getLogPath();
    if (subFolder == "") {
        folder = join(folder, subFolder);
    }

    std::vector<cv::String> filenames;

    glob(folder, "", filenames, false, true); // new function that does the job ;-)
    return filenames;
}

void BoardWriter::removeSubDir() {
    remove_all(subLogDirPath);
}
