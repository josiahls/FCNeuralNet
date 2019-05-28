//
// Created by Laivins, Josiah on 2019-05-21.
//

#ifndef NEURALNETDEMO_DEBUGHELPERS_H
#define NEURALNETDEMO_DEBUGHELPERS_H

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

#endif //NEURALNETDEMO_DEBUGHELPERS_H
