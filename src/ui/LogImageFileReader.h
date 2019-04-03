//
// Created by Laivins, Josiah on 2019-03-21.
//

#ifndef NEURALNETDEMO_LOGIMAGEFILEREADER_H
#define NEURALNETDEMO_LOGIMAGEFILEREADER_H

#include <cstdio>
#include <QObject>
#include <QTextStream>
#include <random>
#include <sys/socket.h>
#include <QValueAxis>
#include <iostream>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include "src/utils/Logger.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QFile>

//QT_CHARTS_BEGIN_NAMESPACE
//class QXYSeries;
//QT_CHARTS_END_NAMESPACE
//QT_CHARTS_USE_NAMESPACE

class LogImageFileReader : public QObject {
    Q_OBJECT
public:

    explicit LogImageFileReader(QGraphicsPixmapItem *imageItem, QGraphicsScene *scene,
            QString logFilePath) :
            m_scene(scene),
            m_imageItem(imageItem),
            m_logFilePath(logFilePath) {
    }

public slots:
    void process() {
        // If the file cannot be opened, then return
        QFile file(this->m_logFilePath);
        // Loop while the file cannot be read
//        std::cout<<"reading image"<< std::endl;
        while (!file.open(QIODevice::ReadOnly | QIODevice::Text)) std::cout << "Can't Read File" << std::endl;

        // Create the text reader and clear the series and buffer for a fresh read
        QTextStream in(&file);
//        m_imageItem->setPixmap(QPixmap::fromImage(QImage()));
//        m_buffer.clear();

        while (true) {
            std::cout<<"reading image"<< std::endl;
            QString line = in.readLine();
            double value = 0;
            double dateTime = 0;

            if(!line.isNull()) {
                // We will use these for moving column wise
                std::stringstream tempStream(line.toStdString());
                std::string temp;
                // Go through each col
                for (int j = 0; std::getline(tempStream, temp, ','); j++) {
                    QString s(temp.c_str());
                    bool isOk = false;
                    if (j == 0) {
                        value = s.toDouble(&isOk);
                        // If the file is a string, it is probably the column title
                        if (!isOk) {
//                            this->m_scene->setTitle(s + " Log");
                        }
                    } else {
                        dateTime = s.toDouble(&isOk);
                    }
                }
                //


            }
        }
    }

signals:
    void finished();
    void error(QString err);

private:
    QGraphicsPixmapItem *m_imageItem;
    QGraphicsScene *m_scene;
    QVector<QPointF> m_buffer;
    QString m_logFilePath;
};

#endif //NEURALNETDEMO_LOGIMAGEFILEREADER_H
