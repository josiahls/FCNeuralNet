//
// Created by Laivins, Josiah on 2019-04-02.
//

#include "ImageLogWidget.h"

QT_CHARTS_USE_NAMESPACE

ImageLogWidget::ImageLogWidget(const QString &logFilePath, QWidget *parent) :
        QWidget(parent),
        m_scene(new QGraphicsScene),
        m_imageItem(new QGraphicsPixmapItem) {
    graphicsView = new QGraphicsView(m_scene);
    graphicsView->setMinimumSize(300, 300);
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