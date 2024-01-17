#include "model/spectrumdatagenerator.h"
#include <QDebug>
#include "utils/readfiledata.h"

QVector<double> *SpectrumDataGenerator::generateBriScatteringData(
    const InputDataListManager *inputDataList)
{
    // Generate spectrum data
    // Assign x and y data accordingly
    for (int i = 0; i < inputDataList->getInputDataList()->size(); i++) {
        qDebug() << "[INFO]: " << inputDataList->getInputDataList()->at(i) << " ";
    }
    qDebug() << Qt::endl;
    QVector<double> *data = new QVector<double>;
    ReadFileData::readCSV(data, "L_bc.csv");
    return data;
}

QVector<double> *SpectrumDataGenerator::generateMieScatteringData(
    const InputDataListManager *inputDataList)
{
    // Generate spectrum data
    // Assign x and y data accordingly
    for (int i = 0; i < inputDataList->getInputDataList()->size(); i++) {
        qDebug() << "[INFO]: " << inputDataList->getInputDataList()->at(i) << " ";
    }
    qDebug() << Qt::endl;
    QVector<double> *data = new QVector<double>;
    ReadFileData::readCSV(data, "L_mc.csv");
    return data;
}

QVector<double> *SpectrumDataGenerator::generateRayScatteringData(
    const InputDataListManager *inputDataList)
{
    // Generate spectrum data
    // Assign x and y data accordingly
    for (int i = 0; i < inputDataList->getInputDataList()->size(); i++) {
        qDebug() << "[INFO]: " << inputDataList->getInputDataList()->at(i) << " ";
    }
    qDebug() << Qt::endl;
    QVector<double> *data = new QVector<double>;
    ReadFileData::readCSV(data, "L_rc.csv");
    return data;
}
