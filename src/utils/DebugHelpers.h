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

    static std::vector<float> unwrapMat(cv::Mat inputMat) {
        std::vector<float> unwrapped;
        if (inputMat.isContinuous()) {
            // array.assign((float*)mat.datastart, (float*)mat.dataend); // <- has problems for sub-matrix like mat = big_mat.row(i)
            unwrapped.assign((float*)inputMat.data, (float*)inputMat.data + inputMat.total());
        } else {
            for (int i = 0; i < inputMat.rows; ++i) {
                unwrapped.insert(unwrapped.end(), inputMat.ptr<float>(i), inputMat.ptr<float>(i)+inputMat.cols);
            }
        }
//        std::printf("Unwrapped Vec: ");
        return unwrapped;
    }

    /**
     * A utility method for showing particularly matrix weights. The intention is for this to be related to
     * image based neural nets, and so will have parameters interested in displaying weights in a image format.
     *
     * @param inputMatrix The matrix to visually display.
     * @param imageDimension The dimension of the image. Useful for reshaping a 1D weight layer into the original
     * image size.
     * @param scale Scaling higher than 1 will enlarge the image, and less than 1 will down size the image.
     */
    static void ImshowMatrixDisplayer(cv::Mat inputMatrix, std::tuple<int, int, int>
            imageDimension=std::tuple<int, int, int>(0, 0, 900), float scale=0, bool block=false,
            bool shouldReshape=true) {
//        std::printf("\nShowing weight output comparison");
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

    /**
     * A utility method for showing particularly matrix weights. The intention is for this to be related to
     * image based neural nets, and so will have parameters interested in displaying weights in a image format.
     *
     * @param inputMatrix The matrix to visually display.
     * @param imageDimension The dimension of the image. Useful for reshaping a 1D weight layer into the original
     * image size.
     * @param scale Scaling higher than 1 will enlarge the image, and less than 1 will down size the image.
     */
    static void ImshowMatrixDisplayer(std::vector<float> vecInputMatrix, std::tuple<int, int, int>
            imageDimension=std::tuple<int, int, int>(0, 0, 900), float scale=0, bool block=false,
            bool shouldReshape=true) {
//        std::printf("\nShowing weight output comparison");
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

    static void print(std::vector<float> &input) {
        for (int i = 0; i < input.size(); i++) {
            std::cout << '\n' << input.at(i) << ' ';
        }
    }
}

#endif //NEURALNETDEMO_DEBUGHELPERS_H
