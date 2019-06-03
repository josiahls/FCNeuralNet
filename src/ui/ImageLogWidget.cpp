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


#include "ImageLogWidget.h"

QT_CHARTS_USE_NAMESPACE

ImageLogWidget::ImageLogWidget(const QString &logFilePath, QWidget *parent) :
        QWidget(parent),
        m_scene(new QGraphicsScene),
        m_imageItem(new QGraphicsPixmapItem) {
    graphicsView = new QGraphicsView(m_scene);
    graphicsView->setMinimumSize(100, 100);
    graphicsView->setMaximumSize(200, 200);
//    graphicsView->setWindowTitle();
    m_scene->addItem(m_imageItem);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    m_label = new QLabel("Start", this);
    m_videoLabel = new QLabel("Loading...", this);
    mainLayout->addWidget(m_label);
    mainLayout->addWidget(m_videoLabel);
//    mainLayout->addWidget(graphicsView);

    m_fileReader = new LogImageFileReader(m_imageItem, m_scene, m_label, m_videoLabel, logFilePath);

    m_thread = new QThread;
    m_fileReader->moveToThread(m_thread);
    connect(m_thread, SIGNAL (started()), m_fileReader, SLOT (process()));
    m_thread->start();
//    this->fileReaderLoopFuture = QtConcurrent::run(this->m_fileReader, &LogImageFileReader::process);
}

ImageLogWidget::~ImageLogWidget() {
    m_fileReadingProcess->close();
}