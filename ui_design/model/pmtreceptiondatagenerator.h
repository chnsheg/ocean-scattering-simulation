#ifndef PMTRECEPTIONDATAGENERATOR_H
#define PMTRECEPTIONDATAGENERATOR_H

#include <QVector>
#include "model/constantmap.h"
#include "model/constantstorage.h"
#include "utils/logger.h"
#include "utils/mymath.h"

class PMTReceptionDataGenerator
{
public:
    PMTReceptionDataGenerator();

    static QVector<QVector<double> *> *generatePMTReceptionData();
    static QVector<QVector<double> *> *receiveSpectrumAfterPMT(QVector<double> *spectrum);
};

#endif // PMTRECEPTIONDATAGENERATOR_H
