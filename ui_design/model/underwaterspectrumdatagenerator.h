#ifndef UNDERWATERSPECTRUMDATAGENERATOR_H
#define UNDERWATERSPECTRUMDATAGENERATOR_H

#include "matlab/CaculateScatteredPhotons.h"
#include <QVector>

class UnderWaterSpectrumDataGenerator
{
public:
    UnderWaterSpectrumDataGenerator();

    static QVector<QVector<double> *> *generateUnderWaterSpectrumData();
    static QVector<double> *caculateScatteredPhotonsByMatlabCode();
    static QVector<QVector<double> *> *generateSNRDepthByMData();
    static QVector<QVector<double> *> *generateSNRDepthByAlphaData();
    static QVector<QVector<double> *> *generateNsMByDepthData();
};

#endif // UNDERWATERSPECTRUMDATAGENERATOR_H
