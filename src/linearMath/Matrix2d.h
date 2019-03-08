//
// Created by Laivins, Josiah on 2019-03-05.
//

#ifndef NEURALNETDEMO_MATRIX2D_H
#define NEURALNETDEMO_MATRIX2D_H

#include "Matrix1d.h"

template <typename T>
struct Matrix2d {
    Matrix2d() {}

    Matrix2d(int nrows, int ncols, T defaultValue)
        : Matrix2d<T>((unsigned long) nrows, (unsigned long)ncols, defaultValue) {}

    unsigned long nrows;
    unsigned long ncols;
    unsigned long size;
    T defaultValue;
    std::vector<Matrix1d<T>> m;

    Matrix2d(unsigned long nrows, unsigned long ncols, T defaultValue) {
        this->nrows = nrows;
        this->ncols = ncols;
        this->size = this->nrows * this->ncols;
        this->defaultValue = defaultValue;
        // Init the matrix
        this->m = std::vector<Matrix1d<T>>(nrows, Matrix1d<T>(ncols, defaultValue));
    }

    Matrix2d(const Matrix2d<T> &copyMatrix) {
        this->nrows = copyMatrix.nrows;
        this->ncols = copyMatrix.ncols;
        this->size = copyMatrix.nrows * copyMatrix.ncols;
        this->defaultValue = copyMatrix.defaultValue;
        // Init the matrix
        this->m = std::vector<Matrix1d<T>>(nrows, Matrix1d<T>(ncols, copyMatrix.defaultValue));
        // Perform full value by value copy
        std::vector<Matrix1d<T>> tempW2 = copyMatrix.m;
        std::vector<T> tempW;
        for(int row = 0; row < nrows; row++) {
            for (int col = 0; col < ncols; col++) {
                tempW.push_back(tempW2[row][col]);
            }
        }
        wrap(tempW, nrows, ncols);
    }

    std::vector<T> unwrap() {
        std::vector<T> tempW;
        for(int row = 0; row < nrows; row++) {
            for (int col = 0; col < ncols; col++) {
                tempW.push_back(m[row][col]);
            }
        }
        return tempW;
    }

    void wrap(std::vector<T> unwrappedMatrix, int otherNRows, int otherNCols) {
        for(int row = 0; row < nrows; row++) {
            for (int col = 0; col < ncols; col++) {
                m[row][col] = unwrappedMatrix[row * col];
            }
        }
    }

    Matrix1d<T>& operator[] (int x) {
        return this->m.at(x);
    }
};

#endif //NEURALNETDEMO_MATRIX2D_H
