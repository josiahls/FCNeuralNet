//
// Created by Laivins, Josiah on 2019-03-21.
//

#ifndef NEURALNETDEMO_LOGFILEREADER_H
#define NEURALNETDEMO_LOGFILEREADER_H

#include <cstdio>
#include <QObject>
#include <QtCharts/QChart>
#include <QLineSeries>
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

QT_CHARTS_BEGIN_NAMESPACE
class QXYSeries;
QT_CHARTS_END_NAMESPACE
QT_CHARTS_USE_NAMESPACE

class LogFileReader  : public QObject {
    Q_OBJECT
public:
    explicit LogFileReader(QLineSeries *series, QChart *chart,QValueAxis *axisX, QValueAxis *axisY,
            QString logFilePath) :
            m_chart(chart),
            m_series(series),
            m_axisX(axisX),
            m_axisY(axisY),
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
        m_series->clear();
        m_buffer.clear();

        while (true) {

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
                            this->m_chart->setTitle(s + " Log");
                            this->m_axisY->setTitleText(s);
                            this->m_axisX->setTitleText("Steps");
                        }
                    } else {
                        dateTime = s.toDouble(&isOk);
                    }
                }
                // Get the time string
                std::cout << line.toStdString() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                // Adding the new point will update the series
                m_buffer.append(QPointF(m_buffer.size() + 1, value));

                // Reset the graph size
                float xMin = std::numeric_limits<float>::max(); // everything is <= this
                float xMax = std::numeric_limits<float>::min(); // everything is >= this
                float yMin = std::numeric_limits<float>::max();
                float yMax = std::numeric_limits<float>::min();
                foreach (QPointF p, m_buffer) {
                    xMin = qMin(xMin, float(p.x()));
                    xMax = qMax(xMax, float(p.x()));
                    yMin = qMin(yMin, float(p.y()));
                    yMax = qMax(yMax, float(p.y()));
                }

                m_axisX->setRange(xMin, xMax);
                m_axisY->setRange(yMin, yMax);
                m_series->replace(m_buffer);
            }
        }
    }

signals:
    void finished();
    void error(QString err);

private:
    QLineSeries *m_series;
    QChart *m_chart;
    QVector<QPointF> m_buffer;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    QString m_logFilePath;
};

#endif //NEURALNETDEMO_LOGFILEREADER_H
