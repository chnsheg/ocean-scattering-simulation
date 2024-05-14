#ifndef FIZEAUIFGENERATOR_H
#define FIZEAUIFGENERATOR_H

#include <QVector>

class FizeauIFGenerator
{
public:
    FizeauIFGenerator();

    static QVector<QVector<double> *> *generateFizeauIFData();
    static QVector<QVector<double> *> *calculateSpectrumAfterFizeau(QVector<double> *fizeau_IF);
    static QVector<double> *getAfterFizeauSpectrumData(QVector<double> *spectrum_data);

public:
    static QVector<double> *Fizeau_Polyarea;
};

#endif // FIZEAUIFGENERATOR_H
