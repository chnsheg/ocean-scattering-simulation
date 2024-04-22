#include "screencapturetask.h"

int ScreenCaptureTask::objectCount = 0;

ScreenCaptureTask::ScreenCaptureTask(int index, QRect captureRect) : index(index), captureRect(captureRect)
{
    objectCount++;
    qDebug() << "ScreenCaptureTask object count:" << objectCount;
}

void ScreenCaptureTask::run()
{
    qDebug() << "Task is running in thread:" << QThread::currentThreadId();

    // 根据objectCount生成对应任务标识符
    QString taskName = "ScreenCaptureTask" + QString::number(objectCount);
    QVariantList *args = new QVariantList{0};

    // 截图并保存到constantstorage中，存储池索引是8
    QScreen *screen = QGuiApplication::primaryScreen();

    if (!screen)
    {
        args->append(-1);
        emit taskCompleted(taskName, args);
        return;
    }

    // 定义截图区域，例如从点(100, 100)开始，宽度和高度均为400像素
    // QRect captureRect(100, 100, 400, 400);

    // 截图指定区域
    qDebug() << "captureRect: " << captureRect;
    QPixmap pixmap = screen->grabWindow(0, captureRect.x(), captureRect.y(), captureRect.width(), captureRect.height());

    if (pixmap.isNull())
    {
        args->append(0);
        emit taskCompleted(taskName, args);
        return;
    }

    QPixmap *pixmapPtr = new QPixmap(pixmap);

    // 先释放之前的图片
    if (Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(8, index)).value<QPixmap *>() != nullptr)
    {
        QPixmap *pixmapPtr = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(8, index)).value<QPixmap *>();
        delete pixmapPtr;
    }

    Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(8, index), QVariant::fromValue(pixmapPtr));

    // 保存图片到文件
    QString filePath = QDir::currentPath() + "/capture.png";

    pixmap.save(filePath);

    args->append(1);
    emit taskCompleted(taskName, args);
}