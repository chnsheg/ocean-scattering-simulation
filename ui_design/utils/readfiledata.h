#ifndef READFILEDATA_H
#define READFILEDATA_H

#include <QDebug>
#include <QFile>
#include <QString>
#include <QVector>
#include <QTextStream>

class ReadFileData
{
public:
    ReadFileData();
    static void readCSV(QVector<double> *data, QString filename);
    static void saveDataToCSVFile(const QVector<double> *xDataVector, const QVector<double> *yDataVector, const QString &fileName);
};

#endif // READFILEDATA_H
