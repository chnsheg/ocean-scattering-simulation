#ifndef SPECTRUMDATAGENERATOR_H
#define SPECTRUMDATAGENERATOR_H

#include <QVector>
#include "inputdatalistmanager.h"

class SpectrumDataGenerator
{
public:
    static QVector<double> *generateBriScatteringData(const InputDataListManager *inputDataList);
    static QVector<double> *generateMieScatteringData(const InputDataListManager *inputDataList);
    static QVector<double> *generateRayScatteringData(const InputDataListManager *inputDataList);
};

#endif // SPECTRUMDATAGENERATOR_H
