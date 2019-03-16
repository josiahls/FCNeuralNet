cmake_minimum_required(VERSION 3.13)
project(NeuralNetDemo)

set(CMAKE_CXX_STANDARD 14)


if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

add_executable(NeuralNetDemo main.cpp src/executables/VisualizationBoard.cpp src/nn/NeuralNet.cpp src/nn/NeuralNet.h src/layers/Dimension.h src/layers/Layer.cpp
                             src/layers/Layer.h src/utils/DatasetCar.h src/ui/MainWindow.h src/ui/MainWindow.cpp
                             src/utils/BoardWriter.h src/executables/NeuralNetRun.cpp src/executables/NeuralNetRun.h src/executables/VisualizationBoard.h)
add_subdirectory(test)

####### Setup Qt #################################
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(QT5Charts CONFIG REQUIRED)

# Use the Widgets module from Qt 5
target_link_libraries(NeuralNetDemo Qt5::Charts)
##################################################

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Link your application with OpenCV libraries
target_link_libraries(NeuralNetDemo ${OpenCV_LIBS})
