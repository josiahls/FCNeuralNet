//
// Created by Laivins, Josiah on 2019-03-12.
//

#ifndef NEURALNETDEMO_LOGGERWIDGET_H
#define NEURALNETDEMO_LOGGERWIDGET_H

#include <QWidget>
#include "LogThread.h"

class LoggerWidget : public QWidget {
    // Your gui code


    void startWorkInAThread() {
        // Create an instance of your woker
        auto *workerThread = new LogWorkerThread;
        // Connect our signal and slot
        connect(workerThread, SIGNAL(progressChanged(QString)),
                SLOT(onProgressChanged(QString)));
        // Setup callback for cleanup when it finishes
        connect(workerThread, SIGNAL(finished()),
                workerThread, SLOT(deleteLater()));
        // Run, Forest, run!
        workerThread->start(); // This invokes WorkerThread::run in a new thread
    }
    // Define slot:
public slots:
            void onProgressChanged(QString info){
                // Processing code
            }
};

#endif //NEURALNETDEMO_LOGGERWIDGET_H
