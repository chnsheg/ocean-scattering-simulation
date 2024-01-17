#include "model/laserdatagenerator.h"
#include <QDebug>
#include "utils/readfiledata.h"

QVector<double> *LaserDataGenerator::generateLaserData(const InputDataListManager *inputDataList)
{
    for (int i = 0; i < inputDataList->getInputDataList()->size(); i++) {
        qDebug() << "[INFO]: " << inputDataList->getInputDataList()->at(i) << " ";
    }
    qDebug() << Qt::endl;
    QVector<double> *data = new QVector<double>;
    ReadFileData::readCSV(data, "laser_data.csv");
    return data;
}
