#ifndef FREQUENCEDATAGENERATOR_H
#define FREQUENCEDATAGENERATOR_H

#include <QVector>
#include "manager/inputdatalistmanager.h"

class FrequenceDataGenerator
{
public:
    static QVector<double> *generateFrequenceData();
};

#endif // FREQUENCEDATAGENERATOR_H
