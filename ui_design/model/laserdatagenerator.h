#ifndef LASERDATAGENERATOR_H
#define LASERDATAGENERATOR_H

#include <QVector>
#include "manager/inputdatalistmanager.h"

class LaserDataGenerator
{
public:
    LaserDataGenerator();
    static QVector<double> *generateLaserData(const InputDataListManager *inputDataList);
};

#endif // LASERDATAGENERATOR_H
