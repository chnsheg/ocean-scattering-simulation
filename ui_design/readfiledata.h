#ifndef READFILEDATA_H
#define READFILEDATA_H

#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>

class ReadFileData
{
public:
    ReadFileData();
    static void readCSV(QVector<double> *data, QString filename);
};

#endif // READFILEDATA_H
