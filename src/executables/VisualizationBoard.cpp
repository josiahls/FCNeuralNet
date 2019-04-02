//
// Created by Laivins, Josiah on 2019-03-15.
//

#include <cstdio>
#include <QApplication>
#include <QMainWindow>
#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
#include <functional>
#include <src/ui/LogWidget.h>

namespace boardViz {
    int run(int argc, char *argv[]) {
        // Init the application
        QApplication a(argc, argv);
        // Setup the Layout for 1+ widgets that might be generated
        QGridLayout *mainLayout = new QGridLayout;

        // Get the most recent directory, and init the the number of graphs in that directory
        std::vector<cv::String> dirs = Logger::getLogDirs();
        std::sort(dirs.begin(), dirs.end());
        cv::String mostRecentDir = dirs.back();
        // Get the files in the directory
        std::vector<cv::String> files = Logger::getLogDirs(mostRecentDir);

        std::vector<std::shared_ptr<QWidget>> logWidgets;
        int i = 0;
        for (auto &item : files) {
                logWidgets.push_back(std::shared_ptr<QWidget>(new LogWidget(item.c_str())));
                mainLayout->addWidget(logWidgets.back().get(), i / 2, (i % 2), 1, 1, Qt::AlignLeft);
                i++;
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