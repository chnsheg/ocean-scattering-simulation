#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include "base/singleton.h"
#include "manager/texteditmanager.h"

class Logger : public QObject
{
    Q_OBJECT
    SINGLETON(Logger)

public:
    // 定义信息等级
    enum LogLevel
    {
        Log,     // 记录系统运行信息和状态
        Warning, // 记录系统运行时的警告信息，例如：输入参数错误
        Info,    // 记录系统运行时的重要信息，例如：输入参数正确
        Title,   // 记录系统运行时的标题信息
    };

private:
    Logger(TextEditManager *textEditManager, QObject *parent = nullptr);

    // 定义一个二级指针，指向TextEditManager的单例指针，用来保证TextEditManager的单例指针指向改变时，Logger的指针不需要改变
    TextEditManager *m_textEditManager;

public:
    // 槽函数，接收外部传入的QString信息和信息等级
    void logMessage(const QString &message, LogLevel level = Log);
    void logMessage(QTextEdit *textEdit, const QString &message, LogLevel level = Log, int fontSize = 12);

private:
    // 根据信息等级获取对应的字符格式
    QTextCharFormat getFormatForLogLevel(LogLevel level);
};

#endif // LOGGER_H
