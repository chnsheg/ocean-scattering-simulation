#ifndef PMTRECEPTIONDATAGENERATOR_H
#define PMTRECEPTIONDATAGENERATOR_H

#include <QVector>
#include "model/constantmap.h"
#include "model/constantstorage.h"
#include "utils/logger.h"
#include "utils/mymath.h"
#include <QThread>
#include "model/fizeauifgenerator.h"

class PMTReceptionDataGenerator : public QThread
{
public:
    PMTReceptionDataGenerator();

    static QVector<QVector<double> *> *generatePMTReceptionData();
    static QVector<double> *receiveSpectrumAfterPMT(QVector<double> *spectrum);
    static void retrievalFormPMT();
    static QVector<double> *retrievalBySpecializePMT(QVector<double> *pmt_data, double z);
};

#endif // PMTRECEPTIONDATAGENERATOR_H
