#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QTextStream>
#include <QTranslator>
#include "base/taskrunner.h"
#include "mainwindow.h"
#include "model/screencapturetask.h"
#include "retrievalwindow.h"

QTranslator tran; //翻译器

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // RetrievalWindow rw;

    QFile file(":/qss.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString css = in.readAll();
        a.setStyleSheet(css);
        file.close();
    }
    else
    {
        qDebug() << "open file failed";
    }

    //加载翻译文件
    tran.load(":/English/English.qm");
    a.installTranslator(&tran);
    // 打印当前线程ID
    // qDebug() << "Main thread ID:" << QThread::currentThreadId();

    // TaskRunner::runTask<ScreenCaptureTask>(); // Start a new task
    MainWindow w;
    w.show();
    // rw.show();
    QThreadPool::globalInstance()->setMaxThreadCount(16);
    return a.exec();
}

/*
重构计划：
1.删除无用的头文件
2.删除无用的注释
3.添加每个函数的注释
4.修改每个类的get方法，使调用方式与容器无关
*/
