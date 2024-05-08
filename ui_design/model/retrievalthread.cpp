#include "retrievalthread.h"

int RetrievalThread::objectCount = 0;

RetrievalThread::RetrievalThread(std::function<void()> callback)
    : callback(callback)
{
    objectCount++;
    qDebug() << "RetrievalThread is created, objectCount:" << objectCount;
}

void RetrievalThread::run()
{
    QString taskName = "RetrievalThread" + QString::number(objectCount);
    QVariantList *args = new QVariantList();
    qDebug() << "RetrievalThread is running in thread:" << QThread::currentThreadId();
    // this->setAutoDelete(false);
    // connect(rw, &RetrievalWindow::closeRetrievalWindow, this, &RetrievalThread::deleteRetrievalThread);
    callback();
    args->append(1);
    emit taskCompleted(taskName, args);
    // 销毁线程
    this->deleteLater();
}
