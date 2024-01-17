#ifndef FIZEAUDATAGENERATOR_H
#define FIZEAUDATAGENERATOR_H

#include <QVector>
#include "manager/inputdatalistmanager.h"

class FizeauDataGenerator
{
public:
    FizeauDataGenerator();
    static QVector<double> *generateFizeauData(const InputDataListManager *inputDataList);
    static QVector<double> *generateSpectrumAfterFizeau(const InputDataListManager *inputDataList);
};

#endif // FIZEAUDATAGENERATOR_H
