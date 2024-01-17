#include "model/frequencedatagenerator.h"
#include <QDebug>
#include "utils/readfiledata.h"

QVector<double> *FrequenceDataGenerator::generateFrequenceData(
    const InputDataListManager *inputDataList)
{
    for (int i = 0; i < inputDataList->getInputDataList()->size(); i++) {
        qDebug() << "[INFO]: " << inputDataList->getInputDataList()->at(i) << " ";
    }
    qDebug() << Qt::endl;
    QVector<double> *data = new QVector<double>;
    ReadFileData::readCSV(data, "RF.csv");
    return data;
}