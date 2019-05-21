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
#include <QLabel>
#include <QThread>
#include <QTimer>
#include <QtConcurrent>

//QT_CHARTS_BEGIN_NAMESPACE
//class QXYSeries;
//QT_CHARTS_END_NAMESPACE
//QT_CHARTS_USE_NAMESPACE

class LogImageFileReader : public QObject {
    Q_OBJECT
public:

    explicit LogImageFileReader(QGraphicsPixmapItem *imageItem, QGraphicsScene *scene, QLabel *label,
            QLabel *videoLabel, QString logFilePath) :
            m_scene(scene),
            m_imageItem(imageItem),
            m_label(label),
            m_videoLabel(videoLabel),
            m_logFilePath(logFilePath) {
    }

public slots:
    void process() {
        // If the file cannot be opened, then return
        QFile file(this->m_logFilePath);
        // Loop while the file cannot be read
        while (!file.open(QIODevice::ReadOnly | QIODevice::Text)) std::cout << "Can't Read File" << std::endl;

        // Create the text reader and clear the series and buffer for a fresh read
        QTextStream in(&file);

        int lineNumber = 0;
        double value = 0;
        double dateTime = 0;

        QThread::sleep(3);
//        m_scene->removeItem(m_imageItem);
        QPixmap p;
        std::vector<QString> filenames;
        std::vector<double> accuracies;

        while (true) {
//            QPixmap p("/Users/jlaivins/CLionProjects/NeuralNetDemo/data/steering_dataset/training/images/0049.jpg");
//            p = p.scaledToHeight(300);
//            m_imageItem->setPixmap(p);

            QString line = in.readLine();

            if (lineNumber++ == 0 ) {
                continue;
            }

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
                        std::string title("Predicted Steering Angle is: " + std::to_string(value) + " frame: " + std::to_string(lineNumber));
                        m_label->setText(title.c_str());
//                        m_label->setText(std::to_string(lineNumber).c_str());
                        // If the file is a string, it is probably the column title
                        if (!isOk) {
                           //this->m_scene->setT(s + " Log");
                        } else {
                            accuracies.push_back(value);
                        }
                    } else if (j == 1) {
                        std::cout << " Loading image " << temp << std::endl;
                        p.load(s);
                        filenames.push_back(s);
                        p = p.scaled(500, 500);
//                        QPixmap p(s);
//                        QPixmap image("/Users/jlaivins/CLionProjects/NeuralNetDemo/data/steering_dataset/training/images/0027.jpg");
//                        p.scaledToHeight(300);
//                        m_imageItem->();
//                        m_scene->removeItem(m_imageItem);
//                        m_imageItem->setPixmap(p);
                        m_videoLabel->setPixmap(p);
//                        m_scene->update();
//                        m_scene->clear();
//                        m_scene->addPixmap(p);
////                        m_scene->addItem(m_imageItem);
//                        m_scene->update();
                    } else {
                        dateTime = s.toDouble(&isOk);
                    }
                }
//                QPixmap p2("/Users/jlaivins/CLionProjects/NeuralNetDemo/data/steering_dataset/training/images/0026.jpg");
//                p2 = p2.scaledToHeight(300);
//                m_imageItem->resetMatrix();
//                m_imageItem->setPixmap(p2);
            }

            // If the line is null, move to looping the video
            if (line.isNull()) {
                // If it passes the filename size, loop it back to 0
                if (lineNumber >= filenames.size()) lineNumber = 0;
                // Update the accuracy label
                std::string title("Predicted Steering Angle is: " + std::to_string(accuracies.at(lineNumber)) +
                    " frame: " + std::to_string(lineNumber));
                m_label->setText(title.c_str());
                // Update the image / video
                p.load(filenames.at(lineNumber));
                p = p.scaled(500, 500);
                m_videoLabel->setPixmap(p);
            }
            // Needs a minimum sleep rate, otherwise closes strangely
            QThread::msleep(50);
        }
    }

signals:
    void finished();
    void error(QString err);

private:
    QGraphicsPixmapItem *m_imageItem;
    QGraphicsScene *m_scene;
    QVector<QPointF> m_buffer;
    QPixmap *m_image;
    QLabel *m_label;
    QLabel *m_videoLabel;
    QString m_logFilePath;
};

#endif //NEURALNETDEMO_LOGIMAGEFILEREADER_H
