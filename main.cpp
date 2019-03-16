
#include <cstdio>
#include <string>
#include "src/executables/NeuralNetRun.h"
#include "src/executables/VisualizationBoard.h"

int main(int argc, char *argv[]) {

    if (argc <= 1) {
        printf("Please use of of the following arguments:\n"
               "-runViz\n"
               "-runNN");
    } else {
        std::string s = std::string(argv[1]);

        if (s == "-runViz") {
            boardViz::run();
        } else if (s == "-runNN") {
            nn::run();
        }

    }

}
