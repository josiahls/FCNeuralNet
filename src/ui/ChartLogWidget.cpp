//
// Created by Laivins, Josiah on 2019-03-19.
//

#include "ChartLogWidget.h"

QT_CHARTS_USE_NAMESPACE

ChartLogWidget::ChartLogWidget(const QString &logFilePath, QWidget *parent) :
        QWidget(parent),
        m_chart(new QChart),
        m_series(new QLineSeries) {
    QChartView *chartView = new QChartView(m_chart);
    chartView->setMinimumSize(300, 300);
    m_chart->addSeries(m_series);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 20);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 6);
    axisY->setTitleText("Audio level");

    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_series->attachAxis(axisX);
    m_series->attachAxis(axisY);


    m_chart->legend()->hide();
    m_chart->setTitle("Data from the microphone (");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(chartView);

    m_fileReader = new LogFileReader(m_series, m_chart, axisX, axisY, logFilePath);
    m_thread = new QThread;
    m_fileReader->moveToThread(m_thread);
    connect(m_thread, SIGNAL (started()), m_fileReader, SLOT (process()));
    m_thread->start();
}

ChartLogWidget::~ChartLogWidget()
{
    m_fileReadingProcess->close();
}