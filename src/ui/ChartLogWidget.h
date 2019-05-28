//
// Created by Laivins, Josiah on 2019-03-17.
//

#ifndef NEURALNETDEMO_LOGWIDGET_H
#define NEURALNETDEMO_LOGWIDGET_H

#include "LogFileReader.h"
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QWidget>
#include <QtCharts/QChartGlobal>
#include <QTextStream>
#include <QFile>
#include <QProcess>
#include <QFuture>

#include <QtWidgets/QVBoxLayout>
#include <QTextStream>
#include <QtConcurrent>

QT_CHARTS_BEGIN_NAMESPACE
    class QLineSeries;
    class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class ChartLogWidget : public QWidget {
Q_OBJECT

public:
    explicit ChartLogWidget(const QString &logFilePath, QWidget *parent = nullptr);

    ~ChartLogWidget();

private:
    QChart *m_chart;
    QLineSeries *m_series ;
    QProcess *m_fileReadingProcess = nullptr;
    LogFileReader *m_fileReader = nullptr;
    QThread *m_thread = nullptr;
    QFuture<void> fileReaderLoopFuture;
};

#endif //NEURALNETDEMO_LOGWIDGET_H
