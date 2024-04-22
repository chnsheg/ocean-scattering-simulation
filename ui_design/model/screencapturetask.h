#ifndef SCREENCAPTURETASK_H
#define SCREENCAPTURETASK_H

#include <QObject>
#include "base/taskrunner.h"
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QDir>
#include "model/constantmap.h"
#include "model/constantstorage.h"
#include "base/singleton.h"

class ScreenCaptureTask : public TaskRunner
{
    Q_OBJECT
public:
    explicit ScreenCaptureTask(int index, QRect captureRect);

    ~ScreenCaptureTask() override
    {
        qDebug() << "ScreenCaptureTask is destroyed";
    }

    // int run(int index, QRect captureRect);

    void run() override;

private:
    int index;
    QRect captureRect;
    static int objectCount;
};

#endif // SCREENCAPTURETASK_H
