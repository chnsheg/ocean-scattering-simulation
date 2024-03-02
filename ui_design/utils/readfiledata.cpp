#include "utils/readfiledata.h"

ReadFileData::ReadFileData() {}

void ReadFileData::readCSV(QVector<double> *data, QString filename)
{
    QFile inFile(":/" + filename);
    QStringList lines; /*行数据*/
    if (inFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream_text(&inFile);
        while (!stream_text.atEnd())
        {
            lines.push_back(stream_text.readLine());
        }
        for (int j = 0; j < lines.size(); j++)
        {
            QString line = lines.at(j);
            QStringList split = line.split(","); /*列数据*/
            for (int col = 0; col < split.size(); col++)
            {
                data->push_back(split.at(col).toDouble());
                // qDebug() << split.at(col).toDouble() << "\t";
            }
        }
        inFile.close();
    }
    else
    {
        qDebug() << "open file failed";
    }
}

void ReadFileData::saveDataToCSVFile(const QVector<double> *xDataVector, const QVector<double> *yDataVector, const QString &fileName)
{
    QFile file{fileName};

    // 先要检查文件后缀名是否为csv
    if (!fileName.endsWith(".csv"))
    {
        qWarning() << "File format error, please save as .csv file";
        return;
    }

    // 文件第一行存入xDataVector的数据，第二行存入yDataVector的数据，并且第一行表头是x，第二行表头是y
    // 打开文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // 处理文件打开失败的情况
        qDebug() << "Failed to open file for writing:" << fileName;
        return;
    }

    // 创建文本流
    QTextStream stream(&file);

    // 写入表头
    stream << "x";
    for (const double &x : *xDataVector)
    {
        stream << "," << x;
    }

    stream << "\n";

    stream << "y";
    for (const double &y : *yDataVector)
    {
        stream << "," << y;
    }
    stream << "\n";

    // 关闭文件
    file.close();

    qDebug() << "Data saved to CSV file:" << fileName;
}