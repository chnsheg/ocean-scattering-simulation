#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QThreadPool>
#include <QObject>
#include <QVariant>

class TaskRunner : public QObject, public QRunnable
{
    Q_OBJECT

public:
    template <typename T, typename... Args>
    static TaskRunner *runTask(Args &&...args);

protected:
    void run() override = 0; // QRunnable's run method is pure virtual

signals:
    // 添加一个任务完成信号，第一个参数是任务标识符，后面为可变参数
    void taskCompleted(QString taskName, QVariantList *args);
};

template <typename T, typename... Args>
TaskRunner *TaskRunner::runTask(Args &&...args)
{
    auto task = new T(std::forward<Args>(args)...);
    task->setAutoDelete(true); // Ensures the task will delete itself after running
    QThreadPool::globalInstance()->start(task);
    return task;
}

#endif // TASKMANAGER_H
