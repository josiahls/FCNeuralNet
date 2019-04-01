//
// Created by Laivins, Josiah on 2019-03-19.
//

#ifndef NEURALNETDEMO_XYSERIESIODEVICE_H
#define NEURALNETDEMO_XYSERIESIODEVICE_H

#include <QtCore/QIODevice>
#include <QtCore/QPointF>
#include <QtCore/QVector>
#include <QtCharts/QChartGlobal>

QT_CHARTS_BEGIN_NAMESPACE
class QXYSeries;
QT_CHARTS_END_NAMESPACE
QT_CHARTS_USE_NAMESPACE

class XYSeriesIODevice : public QIODevice
{
    Q_OBJECT
public:
    explicit XYSeriesIODevice(QXYSeries *series, QObject *parent = nullptr);

    void fileReader();
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;
protected:
private:
    QXYSeries *m_series;
    QVector<QPointF> m_buffer;
};


#endif //NEURALNETDEMO_XYSERIESIODEVICE_H
