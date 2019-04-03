//
// Created by Laivins, Josiah on 2019-04-02.
//

#include "ImageLogWidget.h"

QT_CHARTS_USE_NAMESPACE

ImageLogWidget::ImageLogWidget(const QString &logFilePath, QWidget *parent) :
        QWidget(parent),
        m_scene(new QGraphicsScene),
        m_imageItem(new QGraphicsPixmapItem) {
    QGraphicsView *graphicsView = new QGraphicsView(m_scene);
    graphicsView->setMinimumSize(400, 300);
    m_scene->addItem(m_imageItem);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(graphicsView);

    m_fileReader = new LogImageFileReader(m_imageItem, m_scene, logFilePath, this);
    this->fileReaderLoopFuture = QtConcurrent::run(this->m_fileReader, &LogImageFileReader::readFile);
}

ImageLogWidget::~ImageLogWidget() {
    m_fileReadingProcess->close();
}