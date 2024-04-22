#ifndef RETRIEVALTHREAD_H
#define RETRIEVALTHREAD_H

#include <QObject>
#include "base/taskrunner.h"
#include <QDebug>

class RetrievalThread : public TaskRunner
{
    Q_OBJECT
public:
    explicit RetrievalThread(std::function<void()> callback);

    ~RetrievalThread() override
    {
        qDebug() << "RetrievalThread is destroyed";
    }

    void run() override;

private:
    std::function<void()> callback; // 存储回调函数的函数对象

    static int objectCount;
};

#endif // RETRIEVALTHREAD_H
