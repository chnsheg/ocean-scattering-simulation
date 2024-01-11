#include "spectrumdatagenerator.h"
#include <QDebug>
#include "readfiledata.h"

QVector<double> *SpectrumDataGenerator::generateBriScatteringData(InputDataListManager *inputDataList)
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

QVector<double> *SpectrumDataGenerator::generateMieScatteringData(InputDataListManager *inputDataList)
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

QVector<double> *SpectrumDataGenerator::generateRayScatteringData(InputDataListManager *inputDataList)
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
