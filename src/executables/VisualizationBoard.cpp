//
// Created by Laivins, Josiah on 2019-03-15.
//

#include <cstdio>
#include <QApplication>
#include <QMainWindow>
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
#include <functional>
#include <src/ui/ChartLogWidget.h>
#include <src/ui/ImageLogWidget.h>

namespace boardViz {
    int run(int argc, char *argv[]) {
        // Init the application
        QApplication a(argc, argv);
        // Setup the Layout for 1+ widgets that might be generated
        QGridLayout *mainLayout = new QGridLayout;

        // Get the most recent directory, and init the the number of graphs in that directory
        std::vector<cv::String> dirs = Logger::getLogDirs();
        // If the log directory is empty, wait for a directory to be generated.
        while(dirs.empty()) {
                std::cout << "The log directory is empty. Waiting for a log to be made..." << std::endl;
                dirs = Logger::getLogDirs();
                QThread::sleep(1);
        }

        std::sort(dirs.begin(), dirs.end());
        cv::String mostRecentDir = dirs.back();
        // Get the files in the directory
        std::vector<cv::String> files = Logger::getLogDirs(mostRecentDir);
        // If the file directory is empty, wait for a file to be generated.
        while(files.empty()) {
            std::cout << "The file directory is empty. Waiting for a file to be made..." << std::endl;
            files = Logger::getLogDirs(mostRecentDir);
            QThread::sleep(1);
        }

        std::vector<std::shared_ptr<QWidget>> logWidgets;
        int i = 0;
        for (auto &item : files) {
                if (item.find("image") == cv::String::npos) {
                        logWidgets.push_back(std::shared_ptr<QWidget>(new ChartLogWidget(item.c_str())));
                        mainLayout->addWidget(logWidgets.back().get(), i / 3, (i % 3), 1, 1, Qt::AlignLeft);
                        i++;
                } else {
                        logWidgets.push_back(std::shared_ptr<QWidget>(new ImageLogWidget(item.c_str())));
                        mainLayout->addWidget(logWidgets.back().get(), i / 3, (i % 3), 1, 1, Qt::AlignLeft);
                        i++;
                }
        }

        // Create a widget
        QWidget *mainW = new QWidget;
        mainW->setLayout(mainLayout);

        // Add Above to a scroll Area
        QScrollArea *scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(mainW);
        scrollArea->show();

        return a.exec();
    }
}