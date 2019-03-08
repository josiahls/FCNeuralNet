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

    class Proxy {
    public:
        int row;
        Matrix2dv2<T>& matrix;

        Proxy(int row, Matrix2dv2<T>& m) : row(row), matrix(m) {}

        T operator[] (int col) {
            if (col >= matrix.ncols) {
                throw std::out_of_range("There you are trying to access more columns than there actually exist");
            }
            return matrix.m.at(row * matrix.ncols + col);
        }
    };

    Proxy operator[] (int row) {
        if (row >= nrows) {
            throw std::out_of_range("There you are trying to access more rows than there actually exist");
        }
        return Proxy(row, *this);
    }
};

#endif //NEURALNETDEMO_MATRIX2DV2_H
