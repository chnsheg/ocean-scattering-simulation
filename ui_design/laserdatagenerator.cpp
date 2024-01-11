#include "laserdatagenerator.h"
#include <QDebug>
#include "readfiledata.h"

QVector<double> *LaserDataGenerator::generateLaserData(InputDataListManager *inputDataList)
{
    for (int i = 0; i < inputDataList->getInputDataList()->size(); i++) {
        qDebug() << "[INFO]: " << inputDataList->getInputDataList()->at(i) << " ";
    }
    qDebug() << Qt::endl;
    QVector<double> *data = new QVector<double>;
    ReadFileData::readCSV(data, "laser_data.csv");
    return data;
}
