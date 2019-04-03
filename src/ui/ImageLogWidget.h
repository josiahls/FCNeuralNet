//
// Created by Laivins, Josiah on 2019-04-02.
//

#ifndef NEURALNETDEMO_IMAGELOGWIDGET_H
#define NEURALNETDEMO_IMAGELOGWIDGET_H

#include <QWidget>
#include <QTextStream>
#include <QFile>
#include <QProcess>
#include <QFuture>
#include "xyseriesiodevice.h"
#include "LogImageFileReader.h"

#include <QtWidgets/QVBoxLayout>
#include <QTextStream>
#include <QtConcurrent>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

QT_CHARTS_BEGIN_NAMESPACE
    class QLineSeries;
    class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class ImageLogWidget : public QWidget {
Q_OBJECT

public:
    explicit ImageLogWidget(const QString &logFilePath, QWidget *parent = nullptr);

    ~ImageLogWidget();

private:
    QGraphicsScene *m_scene;
    QGraphicsPixmapItem *m_imageItem ;
    QProcess *m_fileReadingProcess = nullptr;
    LogImageFileReader *m_fileReader = nullptr;
    QFuture<void> fileReaderLoopFuture;
};


#endif //NEURALNETDEMO_IMAGELOGWIDGET_H
