#ifndef RETRIEVALTHREAD_H
#define RETRIEVALTHREAD_H

#include <QObject>
#include "base/taskrunner.h"
#include <QDebug>
#include "retrievalwindow.h"

class RetrievalThread : public TaskRunner
{
    Q_OBJECT
public:
    explicit RetrievalThread(std::function<void()> callback);
    RetrievalThread(std::function<void(int, double, double, double, QVector<QVector<double> *> *)> callback_int, int threadType, double N_Bri, double N_Rayleigh, double SNR, QVector<QVector<double> *> *receivedDataContainer, int memory_index);

    ~RetrievalThread() override
    {
        qDebug() << "RetrievalThread is destroyed";
    }

    void run() override;
    void deleteRetrievalThread() { delete this; }

private:
    std::function<void()> callback;                                                              // 存储回调函数的函数对象
    std::function<void(int, double, double, double, QVector<QVector<double> *> *)> callback_int; // 存储回调函数的函数对象
    int _threadType;
    double _N_Bri;
    double _N_Rayleigh;
    double _SNR;
    QVector<QVector<double> *> *_receivedDataContainer;

    static int objectCount;
    static int receiveThreadIndex;
    int _memory_index;
};

#endif // RETRIEVALTHREAD_H
