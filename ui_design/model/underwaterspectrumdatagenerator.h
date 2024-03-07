#ifndef UNDERWATERSPECTRUMDATAGENERATOR_H
#define UNDERWATERSPECTRUMDATAGENERATOR_H

#include "CaculateScatteredPhotons.h"
#include "model/CaculateScatteredPhotons.h"
#include <QVector>

class UnderWaterSpectrumDataGenerator
{
public:
    UnderWaterSpectrumDataGenerator();

    static QVector<QVector<double> *> *generateUnderWaterSpectrumData();
    static QVector<double> *caculateScatteredPhotonsByMatlabCode();
};

#endif // UNDERWATERSPECTRUMDATAGENERATOR_H
