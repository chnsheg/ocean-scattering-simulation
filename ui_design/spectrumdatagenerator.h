#ifndef SPECTRUMDATAGENERATOR_H
#define SPECTRUMDATAGENERATOR_H

#include <QVector>
#include "inputdatalistmanager.h"

class SpectrumDataGenerator
{
public:
    static QVector<double> *generateBriScatteringData(InputDataListManager *inputDataList);
    static QVector<double> *generateMieScatteringData(InputDataListManager *inputDataList);
    static QVector<double> *generateRayScatteringData(InputDataListManager *inputDataList);
};

#endif // SPECTRUMDATAGENERATOR_H
