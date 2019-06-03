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


#ifndef FCNEURALNET_IMAGELOGWIDGET_H
#define FCNEURALNET_IMAGELOGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextStream>
#include <QFile>
#include <QProcess>
#include <QFuture>
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
    QGraphicsPixmapItem *m_imageItem;
    QPixmap *m_image;
    QProcess *m_fileReadingProcess = nullptr;
    LogImageFileReader *m_fileReader = nullptr;
    QThread *m_thread = nullptr;
    QLabel *m_label = nullptr;
    QLabel *m_videoLabel = nullptr;
    QGraphicsView *graphicsView = nullptr;
    QFuture<void> fileReaderLoopFuture;
};


#endif //FCNEURALNET_IMAGELOGWIDGET_H
