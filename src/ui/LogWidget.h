//
// Created by Laivins, Josiah on 2019-03-17.
//

#ifndef NEURALNETDEMO_LOGWIDGET_H
#define NEURALNETDEMO_LOGWIDGET_H

#include <QWidget>
#include <QtCharts/QChartGlobal>
#include <QTextStream>
#include <QFile>
#include <QProcess>
#include <QFuture>
#include "xyseriesiodevice.h"
#include "LogFileReader.h"

QT_CHARTS_BEGIN_NAMESPACE
    class QLineSeries;
    class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class LogWidget : public QWidget {
Q_OBJECT

public:
    explicit LogWidget(const QString &logFilePath, QWidget *parent = nullptr);

    ~LogWidget();

private:
    QChart *m_chart;
    QLineSeries *m_series ;
    QProcess *m_fileReadingProcess = nullptr;
    LogFileReader *m_fileReader = nullptr;
    QFuture<void> fileReaderLoopFuture;
};

#endif //NEURALNETDEMO_LOGWIDGET_H
