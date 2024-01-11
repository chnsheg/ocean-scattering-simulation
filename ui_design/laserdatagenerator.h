#ifndef LASERDATAGENERATOR_H
#define LASERDATAGENERATOR_H

#include <QVector>
#include "inputdatalistmanager.h"

class LaserDataGenerator
{
public:
    LaserDataGenerator();
    static QVector<double> *generateLaserData(InputDataListManager *inputDataList);
};

#endif // LASERDATAGENERATOR_H
