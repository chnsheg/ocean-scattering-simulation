#include "retrievalthread.h"

int RetrievalThread::objectCount = 0;
int RetrievalThread::receiveThreadIndex = 0;

RetrievalThread::RetrievalThread(std::function<void()> callback)
    : callback(callback)
{
    objectCount++;
    _threadType = 0;
    qRegisterMetaType<QVector<double> *>("QVector<double> *");
    qDebug() << "RetrievalThread is created, objectCount:" << objectCount;
}

RetrievalThread::RetrievalThread(std::function<void(int, double, double, double, QVector<QVector<double> *> *, double)> callback_int, int threadType, double N_Bri, double N_Rayleigh, double SNR, QVector<QVector<double> *> *receivedDataContainer, int memory_index, double depth)
    : callback_int(callback_int), _threadType(threadType), _N_Bri(N_Bri), _N_Rayleigh(N_Rayleigh), _SNR(SNR), _receivedDataContainer(receivedDataContainer), _memory_index(memory_index), _depth(depth)
{
    objectCount++;
    receiveThreadIndex++;
    qRegisterMetaType<QVector<double> *>("QVector<double> *");
    qDebug() << "RetrievalThread is created, objectCount:" << objectCount;
}

void RetrievalThread::run()
{
    QString taskName = "RetrievalThread" + QString::number(objectCount);
    QVariantList *args = new QVariantList();
    qDebug() << "RetrievalThread is running in thread:" << QThread::currentThreadId() << " ThreadType:" << _threadType;
    // this->setAutoDelete(false);
    // connect(rw, &RetrievalWindow::closeRetrievalWindow, this, &RetrievalThread::deleteRetrievalThread);
    if (_threadType == 0)
    {
        callback();
        args->append(1);
        emit taskCompleted(taskName, args);
        // 销毁线程
        this->deleteLater();
    }
    else if (_threadType == 1)
    {
        callback_int(_memory_index, _N_Bri, _N_Rayleigh, _SNR, _receivedDataContainer, _depth);
        args->append(2);
        args->append(_memory_index);
        qDebug() << "_memory_index _depth" << _memory_index << _depth;
        args->append(_depth);
        args->append(QVariant::fromValue(new QVector<double>(*_receivedDataContainer->at(_memory_index))));
        emit taskCompleted(taskName, args);
    }
}
