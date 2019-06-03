/**
 * FCNeuralNet
 * Created by Laivins, Josiah https://josiahls.github.io/ on 2019-03-08
 *
 * This software is provided 'as-is', without any express or implied warranty.
 *
 * In no event will the author(s) be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */


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