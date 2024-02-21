#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QTextStream>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

/*
重构计划：
1.删除无用的头文件
2.删除无用的注释
3.添加每个函数的注释
4.修改每个类的get方法，使调用方式与容器无关
*/