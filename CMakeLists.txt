cmake_minimum_required(VERSION 3.13)
project(NeuralNetDemo)

set(CMAKE_CXX_STANDARD 14)

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

add_executable(NeuralNetDemo main.cpp src/nn/NeuralNet.cpp src/nn/NeuralNet.h src/layers/Dimension.h src/linearMath/Matrix2d.h src/linearMath/Matrix1d.h src/layers/Layer.cpp src/layers/Layer.h)
add_subdirectory(test)