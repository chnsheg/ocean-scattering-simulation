#ifndef FIZEAUIFGENERATOR_H
#define FIZEAUIFGENERATOR_H

#include <QVector>

class FizeauIFGenerator
{
public:
    FizeauIFGenerator();

    static QVector<QVector<double> *> *generateFizeauIFData();
    static QVector<QVector<double> *> *calculateSpectrumAfterFizeau(QVector<double> *fizeau_IF);
};

#endif // FIZEAUIFGENERATOR_H
