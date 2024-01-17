#include "fizeaudatagenerator.h"
#include <QDebug>
#include "readfiledata.h"

FizeauDataGenerator::FizeauDataGenerator() {}

QVector<double> *FizeauDataGenerator::generateFizeauData(const InputDataListManager *inputDataList)
{
    for (int i = 0; i < inputDataList->getInputDataList()->size(); i++) {
        qDebug() << "[INFO]: " << inputDataList->getInputDataList()->at(i) << " ";
    }
    qDebug() << Qt::endl;
    QVector<double> *data = new QVector<double>;
    ReadFileData::readCSV(data, "Fizeau_spectrum.csv");
    return data;
}

QVector<double> *FizeauDataGenerator::generateSpectrumAfterFizeau(
    const InputDataListManager *inputDataList)
{
    for (int i = 0; i < inputDataList->getInputDataList()->size(); i++) {
        qDebug() << "[INFO]: " << inputDataList->getInputDataList()->at(i) << " ";
    }
    qDebug() << Qt::endl;
    QVector<double> *data = new QVector<double>;
    ReadFileData::readCSV(data, "SpectrumAfterFizeau.csv");
    return data;
}
