#ifndef LASERDATAGENERATOR_H
#define LASERDATAGENERATOR_H

#include <QVector>

class LaserDataGenerator
{
public:
    LaserDataGenerator();
    static void generateLaserData(QVector<double> *data);
};

#endif // LASERDATAGENERATOR_H
