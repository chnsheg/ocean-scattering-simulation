#include "logger.h"

Logger::Logger() {}

Logger::~Logger() {}

Logger::Logger(TextEditManager *textEditManager, QObject *parent)
    : QObject(parent), m_textEditManager(textEditManager)
{
}

void Logger::logMessage(const QString &message, LogLevel level)
{
    // 获取对应等级的字符格式
    QTextCharFormat format = getFormatForLogLevel(level);

    // 在TextEditManager中添加带有格式的信息
    if (m_textEditManager)
    {
        // (m_textEditManager)->appendText(message, format);
        m_textEditManager->appendText(format.toolTip() + message, format);
    }
}

QTextCharFormat Logger::getFormatForLogLevel(LogLevel level)
{
    QTextCharFormat format;
    switch (level)
    {
    case Log:
        format.setForeground(Qt::darkBlue); // 设置为黑色
        format.setToolTip("[LOG]: ");       // 设置标识符
        break;
    case Warning:
        format.setForeground(Qt::darkRed); // 设置为深红色
        format.setToolTip("[WARNING]: ");  // 设置标识符
        break;
    case Info:
        format.setForeground(Qt::darkGreen); // 设置为深绿色
        format.setToolTip("[INFO]: ");       // 设置标识符
        break;
    case Title:
        format.setForeground(Qt::black); // 设置为深蓝色
        format.setToolTip("[TITLE]: ");  // 设置标识符
        // 设置字体大小
        format.setFontPointSize(16);
        // 设置字体加粗
        format.setFontWeight(99);
        break;
    default:
        break;
    }
    return format;
}