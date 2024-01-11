#ifndef SPECTRUMDATAGENERATOR_H
#define SPECTRUMDATAGENERATOR_H

#include <QVector>

class SpectrumDataGenerator
{
public:
    SpectrumDataGenerator();
    static void generateBriScatteringData(QVector<double> *data);
    static void generateMieScatteringData(QVector<double> *data);
    static void generateRayScatteringData(QVector<double> *data);
};

#endif // SPECTRUMDATAGENERATOR_H
