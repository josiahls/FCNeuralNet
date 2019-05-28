cmake_minimum_required(VERSION 3.13)
project(NeuralNetDemo)

set(CMAKE_CXX_STANDARD 14)


if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

####### Setup Qt #################################
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Widgets Concurrent Charts REQUIRED)

set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
#
## set up a mapping so that the Release configuration for the Qt imported target is
## used in the COVERAGE CMake configuration.
set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

set(SRC_FILES
        ${PROJECT_SOURCE_DIR}/src/utils/DebugHelpers.cpp
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/src/executables/VisualizationBoard.cpp
        ${PROJECT_SOURCE_DIR}/src/nn/NeuralNet.cpp
        ${PROJECT_SOURCE_DIR}/src/layers/Dimension.h
        ${PROJECT_SOURCE_DIR}/src/layers/Layer.cpp
        ${PROJECT_SOURCE_DIR}/src/utils/BoardWriter.cpp
        ${PROJECT_SOURCE_DIR}/src/utils/Logger.cpp
        ${PROJECT_SOURCE_DIR}/src/utils/DatasetCar.cpp
        ${PROJECT_SOURCE_DIR}/src/executables/NeuralNetRun.cpp
        ${PROJECT_SOURCE_DIR}/src/ui/LogImageFileReader.cpp
        ${PROJECT_SOURCE_DIR}/src/ui/LogFileReader.cpp
        ${PROJECT_SOURCE_DIR}/src/ui/ChartLogWidget.cpp
        ${PROJECT_SOURCE_DIR}/src/ui/ImageLogWidget.cpp)

add_executable(NeuralNetDemo ${SRC_FILES})
add_subdirectory(test)

# Use the Widgets module from Qt 5
target_link_libraries(NeuralNetDemo Qt5::Charts)
target_link_libraries(NeuralNetDemo Qt5::Concurrent)
target_link_libraries(NeuralNetDemo Qt5::Widgets)
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
