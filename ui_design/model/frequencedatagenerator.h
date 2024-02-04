#ifndef FREQUENCEDATAGENERATOR_H
#define FREQUENCEDATAGENERATOR_H

#include <QVector>
#include "manager/inputdatalistmanager.h"

class FrequenceDataGenerator
{
public:
    static QVector<double> *generateFrequenceData();
    static QVector<double> *generateRelativeFrequenceData();

private:
    static QVector<double> *data;
    static QVector<double> *relativeData;
    // static int frequence_count;
    // static int relative_frequence_count;
};

#endif // FREQUENCEDATAGENERATOR_H
