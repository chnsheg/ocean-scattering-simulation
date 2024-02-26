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
};

#endif // SPECTRUMDATAGENERATOR_H
