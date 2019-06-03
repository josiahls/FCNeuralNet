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
