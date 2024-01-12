#ifndef FREQUENCEDATAGENERATOR_H
#define FREQUENCEDATAGENERATOR_H

#include <QVector>
#include "inputdatalistmanager.h"

class FrequenceDataGenerator
{
public:
    static QVector<double> *generateFrequenceData(const InputDataListManager *inputDataList);
};

#endif // FREQUENCEDATAGENERATOR_H
