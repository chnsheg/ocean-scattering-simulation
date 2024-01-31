#include "model/frequencedatagenerator.h"
#include <QDebug>
#include "utils/readfiledata.h"

QVector<double> *FrequenceDataGenerator::generateFrequenceData()
{
    QVector<double> *data = new QVector<double>;
    ReadFileData::readCSV(data, "RF.csv");
    return data;
}
