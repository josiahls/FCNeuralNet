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


#ifndef FCNEURALNET_DEBUGHELPERS_H
#define FCNEURALNET_DEBUGHELPERS_H

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>
#include <iostream>

namespace debug {

    /**
     * Returns a vector representing a matrix. Useful for debugging.
     *
     *
     * @param inputMat Opencv Matrix we want to debug
     * @return The matrix unwrapped into a vector.
     */
    std::vector<float> unwrapMat(cv::Mat inputMat);

    /**
     * A utility method for showing particularly matrix weights. The intention is for this to be related to
     * image based neural nets, and so will have parameters interested in displaying weights in a image format.
     *
     * @param inputMatrix The matrix to visually display.
     * @param imageDimension The dimension of the image. Useful for reshaping a 1D weight layer into the original
     * image size.
     * @param scale Scaling higher than 1 will enlarge the image, and less than 1 will down size the image.
     */
    void ImshowMatrixDisplayer(cv::Mat inputMatrix, std::tuple<int, int, int>
            imageDimension=std::tuple<int, int, int>(0, 0, 900), float scale=0, bool block=false,
            bool shouldReshape=true);

    /**
     * A utility method for showing particularly matrix weights. The intention is for this to be related to
     * image based neural nets, and so will have parameters interested in displaying weights in a image format.
     *
     * @param inputMatrix The matrix to visually display.
     * @param imageDimension The dimension of the image. Useful for reshaping a 1D weight layer into the original
     * image size.
     * @param scale Scaling higher than 1 will enlarge the image, and less than 1 will down size the image.
     */
    void ImshowMatrixDisplayer(std::vector<float> vecInputMatrix, std::tuple<int, int, int>
            imageDimension=std::tuple<int, int, int>(0, 0, 900), float scale=0, bool block=false,
            bool shouldReshape=true);

    /**
     * Outputs the input vector
     *
     * @param input The vector we want to output.
     */
    void print(std::vector<float> &input);
}

#endif //FCNEURALNET_DEBUGHELPERS_H
