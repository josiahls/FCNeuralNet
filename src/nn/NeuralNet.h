//
// Created by Laivins, Josiah on 2019-03-05.
//

#ifndef NEURALNETDEMO_NEURALNET_H
#define NEURALNETDEMO_NEURALNET_H

#include <iostream>
#include <vector>
#include "../layers/Layer.h"


using namespace std;


class NeuralNet {
private:
    vector<Layer> layers;
    vector<int> rmse;
    vector<int> cost;
    int maxY;
    int maxX;

public:
    NeuralNet() {
        this->layers = vector<Layer>();
        this->rmse = vector<int>();
        this->cost = vector<int>();
        this->maxY = 0;
        this->maxX = 0;
    };






};


#endif //NEURALNETDEMO_NEURALNET_H
