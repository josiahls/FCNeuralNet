//
// Created by Laivins, Josiah on 2019-04-01.
//

#include "Logger.h"

cv::String Logger::projectRootName = "FCNeuralNet";

std::string Logger::getCurrentPath() {
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

std::string Logger::getLogPath(cv::String rootLogDirName) {
    std::string currentPath = getCurrentPath();
    std::string::size_type index = currentPath.rfind(projectRootName);
    std::string projectPath = currentPath.substr(0, index + projectRootName.size() + 1);
    std::string projectLogPath = join(projectPath, rootLogDirName);

    // As a note, the cv::String constructor, maybe based on the version I am using,
    // requires character arrays
    if (!exists(projectLogPath.c_str())) {
        printf("\nMissing log directory. Making.");
        cv::utils::fs::createDirectory(projectLogPath);
    }

    return projectLogPath;
}

std::vector<cv::String> Logger::getLogDirs(cv::String rootLogDirName, std::string subFolder) {
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
