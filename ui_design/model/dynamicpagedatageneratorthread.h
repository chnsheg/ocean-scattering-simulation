#ifndef DYNAMICPAGEDATAGENERATORTHREAD_H
#define DYNAMICPAGEDATAGENERATORTHREAD_H

#include <QObject>
#include "base/taskrunner.h"
#include <QDebug>
#include "model/pagedatagenerator.h"

Q_DECLARE_METATYPE(QVector<QVector<double> *> *)

class DynamicPageDataGeneratorThread : public TaskRunner
{
    Q_OBJECT
public:
    explicit DynamicPageDataGeneratorThread(int index);

    ~DynamicPageDataGeneratorThread() override
    {
        // 清除所有连接的信号与槽
        PageDataGenerator *pageDataGenerator = Singleton<PageDataGenerator>::getInstance(nullptr);
        // disconnect(this, &taskCompleted, pageDataGenerator, &PageDataGenerator::handleTaskCompletedSlot);
        disconnect(this, &DynamicPageDataGeneratorThread::taskCompleted, pageDataGenerator, &PageDataGenerator::handleTaskCompletedSlot);
        objectCount--;
        qDebug() << "DynamicPageDataGeneratorThread is destroyed";
    }

    void run() override;

private:
    static int objectCount;
    int _index;
};

#endif // DYNAMICPAGEDATAGENERATORTHREAD_H
