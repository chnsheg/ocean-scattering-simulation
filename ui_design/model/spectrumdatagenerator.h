#ifndef SPECTRUMDATAGENERATOR_H
#define SPECTRUMDATAGENERATOR_H

#include <QVector>
#include "model/constantstorage.h"
#include "base/singleton.h"

class SpectrumDataGenerator
{
public:
    static double BrillouinLine();
    static double BrillouinShift();
    static QVector<double> *generateBriScatteringData();
    static QVector<double> *generateMieScatteringData();
    static QVector<double> *generateRayScatteringData();
    static QVector<QVector<double> *> *generateSpectrumDataByMatlabCode();
    // 计算激光线宽对三种散射谱的影响
    static QVector<QVector<double> *> *generateLaserLineWidthEffectData();
};

#endif // SPECTRUMDATAGENERATOR_H
