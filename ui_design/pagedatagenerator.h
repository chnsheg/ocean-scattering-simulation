#ifndef PAGEDATAGENERATOR_H
#define PAGEDATAGENERATOR_H

/*****************************数据层*****************************/

#include <QObject>
#include <QVector>
#include "inputdatalistmanager.h"

class PageDataGenerator : public QObject
{
    Q_OBJECT
public:
    enum DataType {
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

    static PageDataGenerator *getPageDataGeneratorInstance();
    static void destroyPageDataGeneratorInstance();

    QVector<double> *generateData(DataType dataType, const InputDataListManager *inputDataList);

signals:
    void dataGenerated(QVector<QVector<double> *> *xDataVector,
                       QVector<QVector<double> *> *yDataVector,
                       int curve_num);
public slots:
    void generatePairOfData(int page_index, const InputDataListManager *inputDataList);

private:
    explicit PageDataGenerator(QObject *parent = nullptr);
    ~PageDataGenerator();
    static PageDataGenerator *pageDataGeneratorInstance;
    // 禁止拷贝构造和赋值操作
    PageDataGenerator(const PageDataGenerator &other) = delete;
    PageDataGenerator &operator=(const PageDataGenerator &other) = delete;
};

#endif // PAGEDATAGENERATOR_H
