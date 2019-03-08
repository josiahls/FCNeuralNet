//
// Created by Laivins, Josiah on 2019-03-07.
//

#ifndef NEURALNETDEMO_MATRIX1D_H
#define NEURALNETDEMO_MATRIX1D_H

#include "vector"

template <typename T>
struct Matrix1d {
    unsigned long size;
    std::vector<T> m;

    Matrix1d(unsigned long nelements, T defaultValue) {
        this->size = nelements;
        // Init the matrix
        this->m =  std::vector<T>(nelements, defaultValue);
    }

    Matrix1d(std::vector<T> subVector) {
        this->size = subVector.size();
        this->m = subVector;
    }

    Matrix1d() {}

    T &operator[] (int x) {
        return this->m.at(x);
    }
};

#endif //NEURALNETDEMO_MATRIX1D_H
