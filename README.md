## Summary

Fully connect neural network implementation for steering angle proposal based on image input.
This was loosely adapted from [deep_learning](https://github.com/unccv/deep_learning) which I create
a Python neural network from scratch (using numpy as the primary dependency).

I was motivated to convert this to C++ because I have always wanted a reason to create a project
in C++. However most of the projects recently I have wanted to do required fast prototyping. This
was an awesome opportunity since the Python implementation had so few dependencies to use.

Neural Nets typically consist of stacked layers of neurons (the simply ones). There are many different
types of layer configurations such as convolutional, dropout, fully connected, etc. This repository only
contains a fully connected implementation. However using convolutional layers for image machine learning
would most likely have better performance. 

This repo contains:
 - Fully connected Layer
 - 2 Activation functions:
     - Sigmoid
     - Tanh
 - Adam optimizer back prop
 - QT visualization application
 
As you will find in the `NeuralNetRun.cpp` file, I used tanh activation on 2 layer inputs.
One of the challenges was replacing numpy. I solved this by using Opencv as the only
dependency, and leveraged its matrix manipulation functions and os file io operations. 

-----------------

## Contents

* [Installation](#Installation)
* [Running](#Running)
* [Project Structure](#Project Structure)
* [Links](#links)

-----------------

## Installation

This repo requires only 2 dependencies (I am be felicitous, both of these are massive).
- Opencv 3.4.3
- QT 5.X (I used 5.12.1)

I wish I could give better info for both in terms of installing. However, there were
many steps for both (especially QT). I think in the next C++ repos, I will be using 
Python Jupyter / Tensorboard for all my visualizations, maybe even some kind of Python / C++ binding.

The best I can do is provide links for installing both, and other websites I found useful.
Just keep in mind that CMake needs to be able to execute `find_package(OpenCV REQUIRED)` and
`find_package(Qt5 COMPONENTS Core Widgets Concurrent Charts REQUIRED)`. 

[QT Installation](https://doc.qt.io/)
I did most of this code on a Mac, and tried using brew. I do not recommend it.
It would be better to just use the installer. Opencv was easier to install, but is still involved.

[OpenCV Installation for iOS](https://docs.opencv.org/3.4.3/d5/da3/tutorial_ios_install.html)

[OpenCV Installation](https://docs.opencv.org/3.4.3/df/d65/tutorial_table_of_content_introduction.html)

Once done pull the repo from github:
`git clone https://github.com/josiahls/FCNeuralNet`

`cd FCNeuralNet`

`cmake CMakeLists.txt` 
or as I found out using my Mac, I needed to specify the direct path to 
the correct cmake (default was 13.12.X), exeample path goes to the currect one:
`/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" /Users/jlaivins/CLionProjects/FCNeuralNet`

`make`

Then Hurray! You installed it! This CMakeLists file also is fancy in that it will install
Google Test for you! 

## Running

A few things before you can run this. You need to follow the Data section at 
[deep_learning (the readme)](https://github.com/josiahls/deep_learning/tree/master/challenge).

The data folder should have the structure:
```bash
FCNeuralNet
├── data
│   └── training
│       ├── images
│       │   ├── 0000.jpg
│       │   ├── 0001.jpg
│       │   ├── ...
│       │   └── 1499.jpg
│       └── steering_angles.csv
```

Once you have data working, we can now determine if your installations work. You should be able to easily run:

For training the Neural Net:
`./FCNeuralNet -trainNN`
Feel free to go into `/src/executables/NeuralNetRun.cpp` and reference the `train` method. I think that the neural net
performance can be improved by changing the bin size (make larger), adding extra layers (more advanced features?),
and adding more neurons.

`./FCNeuralNet -runNN`
Also free to go into `/src/executables/NeuralNetRun.cpp` and reference the `run` method. However, this method
will produce all the final runs for the visualizer so I would not really want to change much.

`./FCNeuralNet -runVis`


## Project Structure
```bash
.  # Should be FCNeuralNet
├── CMakeLists.txt
├── LICENSE
├── README.md
├── data      # Does not come with the git clone. Reference the Running section.
├── main.cpp  # Main Entry Points
├── src
│   ├── executables
│   │   ├── NeuralNetRun.cpp
│   │   ├── NeuralNetRun.h
│   │   ├── VisualizationBoard.cpp
│   │   └── VisualizationBoard.h
│   ├── layers
│   │   ├── Dimension.h
│   │   ├── Layer.cpp
│   │   └── Layer.h
│   ├── nn
│   │   ├── NeuralNet.cpp
│   │   └── NeuralNet.h
│   ├── ui
│   │   ├── ChartLogWidget.cpp
│   │   ├── ChartLogWidget.h
│   │   ├── ImageLogWidget.cpp
│   │   ├── ImageLogWidget.h
│   │   ├── LogFileReader.cpp
│   │   ├── LogFileReader.h
│   │   ├── LogImageFileReader.cpp
│   │   └── LogImageFileReader.h
│   └── utils
│       ├── BoardWriter.cpp
│       ├── BoardWriter.h
│       ├── DatasetCar.cpp
│       ├── DatasetCar.h
│       ├── DebugHelpers.cpp
│       ├── DebugHelpers.h
│       ├── Logger.cpp
│       └── Logger.h
└── test
    ├── BoardWriterTest.cpp
    ├── CMakeLists.txt
    ├── DatasetTest.cpp
    ├── LayerTest.cpp
    ├── Matrix2dTest.cpp
    ├── NeuralNetTest.cpp
    └── OpenCVUnitTest.cpp
```

## Contribution guidelines

## For more information

## License

[MIT License](LICENSE)