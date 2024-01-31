#ifndef PAGEDATAGENERATOR_H
#define PAGEDATAGENERATOR_H

/*****************************数据层*****************************/

#include <QObject>
#include <QVector>
#include "base/singleton.h"
#include "manager/inputdatalistmanager.h"

class PageDataGenerator : public QObject
{
    Q_OBJECT
    SINGLETON(PageDataGenerator)
public:
    enum DataType
    {
        Frequence,
        Laser,
        MieScattering,
        BriScattering,
        RayScattering,
        UnderwaterScattering,
        FizeauInstrument,
        FizeauSpectra,
        PMTFrequency,
        PMTEnergy
        // Add more page types if needed
    };

    QVector<double> *generateData(DataType dataType);

signals:
    void dataGenerated(QVector<QVector<double> *> *xDataVector,
                       QVector<QVector<double> *> *yDataVector,
                       int curve_num);
public slots:
    void generatePairOfData(const int page_index);

private:
    explicit PageDataGenerator(QObject *parent = nullptr);
    void parseInputDataList(const InputDataListManager *inputDataList);
};

#endif // PAGEDATAGENERATOR_H
