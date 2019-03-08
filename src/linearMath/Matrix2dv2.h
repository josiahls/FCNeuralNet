//
// Created by Laivins, Josiah on 2019-03-07.
//

#ifndef NEURALNETDEMO_MATRIX2DV2_H
#define NEURALNETDEMO_MATRIX2DV2_H

#include <vector>
#include <stdexcept>

template <typename T>
struct Matrix2dv2 {
    Matrix2dv2() {}

    Matrix2dv2(int nrows, int ncols, T defaultValue)
            : Matrix2dv2<T>((unsigned long) nrows, (unsigned long)ncols, defaultValue) {}

    unsigned long nrows;
    unsigned long ncols;
    unsigned long size;
    T defaultValue;
    std::vector<T> m;

    Matrix2dv2(unsigned long nrows, unsigned long ncols, T defaultValue) {
        this->nrows = nrows;
        this->ncols = ncols;
        this->size = this->nrows * this->ncols;
        this->defaultValue = defaultValue;
        // Init the matrix
        this->m = std::vector<T>(this->size, defaultValue);
    }

    Matrix2dv2(const Matrix2dv2<T> &copyMatrix) {
        this->nrows = copyMatrix.nrows;
        this->ncols = copyMatrix.ncols;
        this->size = copyMatrix.nrows * copyMatrix.ncols;
        this->defaultValue = copyMatrix.defaultValue;
        // Init the matrix
        this->m = std::vector<T>(size, copyMatrix.defaultValue);
        // Perform full value by value copy
        std::vector<Matrix1d<T>> tempW2 = copyMatrix.m;
        for(int row = 0; row < nrows; row++) {
            for (int col = 0; col < ncols; col++) {
                this->m[row][col] = tempW2[row][col];
            }
        }
    }

    class Proxy {
    public:
        int row;
        int col;
        Matrix2dv2<T>& matrix;

        Proxy(int row, Matrix2dv2<T>& m) : row(row), matrix(m) {}

        Proxy& operator[] (int col) {
            if (col >= matrix.ncols) {
                throw std::out_of_range("There you are trying to access more columns than there actually exist");
            }
            this->col = col;
            return matrix.m.at(row * matrix.ncols + col);
        }
    };

    Proxy operator[] (int row) {
        if (row >= nrows) {
            throw std::out_of_range("There you are trying to access more rows than there actually exist");
        }
        return Proxy(row, *this);
    }

//    void set(int row, int col) {
//        this->m[row * this->ncols + col]
//    }
};

#endif //NEURALNETDEMO_MATRIX2DV2_H
