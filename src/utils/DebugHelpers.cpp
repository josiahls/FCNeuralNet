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


#include "DebugHelpers.h"

void debug::print(std::vector<float> &input) {
    for (int i = 0; i < input.size(); i++) {
        std::cout << '\n' << input.at(i) << ' ';
    }
}

void debug::ImshowMatrixDisplayer(std::vector<float> vecInputMatrix, std::tuple<int, int, int> imageDimension, float scale,
                             bool block, bool shouldReshape) {
    cv::Mat dst;
    cv::Mat inputMatrix = cv::Mat(vecInputMatrix);
    cv::normalize(inputMatrix, dst, 0, 1, cv::NORM_MINMAX);

    if (shouldReshape) {
        // If the it is possible to reshape the weight matrix to the original image dimensions, then do that,
        // otherwise we will attempt a square of some kind...
        if (dst.rows % std::get<2>(imageDimension) == 0) {
            dst = dst.reshape(std::get<2>(imageDimension), std::get<0>(imageDimension));
        } else {
            // Convert the Mat into 1 row, N columns
            if (dst.cols == 1) dst = dst.reshape(1, dst.cols);

            // We can't use the image dimensions for reshaping... :(
            // so instead we look for a divisor that outputs half the dimensions of the rows and columns
            // and 1 channel
            auto i = 1;
            for (; i < dst.cols; i++) if (dst.cols / ((float)i) < cv::sqrt(dst.cols) && dst.cols % i == 0) break;
            dst = dst.reshape(1, i);
        }
    }

    // Scaling operations depending on if we are scaling up or down
    if (scale <= 1) {
        cv::resize(dst, dst, cv::Size(), scale, scale, cv::INTER_AREA);
    } else {
        cv::resize(dst, dst, cv::Size(), scale, scale, cv::INTER_CUBIC);
    }
    cv::imshow("result",dst);
    if (block) {
        cv::waitKey(0);
    } else {
        cv::waitKey(30);
    }
}

void debug::ImshowMatrixDisplayer(cv::Mat inputMatrix, std::tuple<int, int, int> imageDimension, float scale, bool block,
                             bool shouldReshape) {
    cv::Mat dst;
    cv::normalize(inputMatrix, dst, 0, 1, cv::NORM_MINMAX);

    if (shouldReshape) {
        // If the it is possible to reshape the weight matrix to the original image dimensions, then do that,
        // otherwise we will attempt a square of some kind...
        if (dst.rows % std::get<2>(imageDimension) == 0) {
            dst = dst.reshape(std::get<2>(imageDimension), std::get<0>(imageDimension));
        } else {
            // We can't use the image dimensions for reshaping... :(
            // so instead we look for a divisor that outputs half the dimensions of the rows and columns
            // and 1 channel
            auto i = 1;
            for (; i < dst.cols / 2; i++) if (dst.cols / (float) i < dst.cols * .5) break;
            dst = dst.reshape(1, dst.cols / i);
        }
    }

    // Scaling operations depending on if we are scaling up or down
    if (scale <= 1) {
        cv::resize(dst, dst, cv::Size(), scale, scale, cv::INTER_AREA);
    } else {
        cv::resize(dst, dst, cv::Size(), scale, scale, cv::INTER_CUBIC);
    }
    cv::imshow("result",dst);
    if (block) {
        cv::waitKey(0);
    } else {
        cv::waitKey(30);
    }
}

std::vector<float> debug::unwrapMat(cv::Mat inputMat) {
    std::vector<float> unwrapped;
    if (inputMat.isContinuous()) {
        // array.assign((float*)mat.datastart, (float*)mat.dataend); // <- has problems for sub-matrix like mat = big_mat.row(i)
        unwrapped.assign((float*)inputMat.data, (float*)inputMat.data + inputMat.total());
    } else {
        for (int i = 0; i < inputMat.rows; ++i) {
            unwrapped.insert(unwrapped.end(), inputMat.ptr<float>(i), inputMat.ptr<float>(i)+inputMat.cols);
        }
    }
    return unwrapped;
}
