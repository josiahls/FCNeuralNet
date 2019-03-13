//
// Created by Laivins, Josiah on 2019-03-12.
//

#ifndef NEURALNETDEMO_QTHREAD_H
#define NEURALNETDEMO_QTHREAD_H

#include <QtGui>
#include <QThread>
#include <iostream>

class LogWorkerThread : public QThread {
    void run() {
        while(1) {
            // ... hard work
            std::cout << "hello" << endl;
            // Now want to notify main thread:
            emit progressChanged("Some info");
        }
    }
    // Define signal:
    signals:
            void progressChanged(QString info);
};

#endif //NEURALNETDEMO_QTHREAD_H
