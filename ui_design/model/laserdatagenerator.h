#ifndef LASERDATAGENERATOR_H
#define LASERDATAGENERATOR_H

#include <QVector>
#include "manager/inputdatalistmanager.h"
#include "model/constantstorage.h"
#include "base/singleton.h"

class LaserDataGenerator
{
public:
    LaserDataGenerator();
    static QVector<double> *generateLaserData();
};

#endif // LASERDATAGENERATOR_H
