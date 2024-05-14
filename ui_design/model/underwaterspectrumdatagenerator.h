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
    static QVector<QVector<double> *> *getNsByDepthData();
    static QVector<double> *getUnderWaterSpectrumDataByNAndSNR(double N_Brillouin, double N_Rayleigh, double SNR);
    // QVector<QVector<QVector<double> *> *> *generateUnderWaterSpectrumDataByDepth();
};

#endif // UNDERWATERSPECTRUMDATAGENERATOR_H
