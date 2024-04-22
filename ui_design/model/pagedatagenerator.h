#ifndef PAGEDATAGENERATOR_H
#define PAGEDATAGENERATOR_H

/*****************************数据层*****************************/

#include <QObject>
#include <QVector>
#include "base/singleton.h"
#include "manager/inputdatalistmanager.h"
#include "module/qcustomplot.h"
#include "model/screencapturetask.h"
#include "base/taskrunner.h"

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
    void generateDynamicData(int index);
    void generateDynamicAction(int index);
    int captureImageData(int index, QRect captureRect); // 用于捕获绘图界面的图像
    int getImageData(int index, QPixmap *pixmap);
    int getInfoData(int index, QMap<QString, QVariant> *info);

signals:
    void dataGenerated(QVector<QVector<double> *> *xDataVector,
                       QVector<QVector<double> *> *yDataVector,
                       int curve_num);

    void dynamicDataGenerated(QVector<QVector<double> *> *xDataVector,
                              QVector<QVector<double> *> *yDataVector,
                              int index,
                              QString title,
                              QStringList legendList = QStringList());

    void dataGenerateFinished();

    void actionGenerateFinished();

    void importConstantCompleted(const int page_index, const QVector<QVector<QVector<double> *> *> *xDataVector,
                                 const QVector<QVector<QVector<double> *> *> *yDataVector);

public slots:
    void generatePairOfData(const int page_index);
    void storeRuntimeDataByIndex(QSharedPointer<QCPGraphDataContainer> dataContainer, const int page_index, const int curve_index, int page_type = 0); // 0 表示主要的绘图界面；1表示扩展的绘图界面
    void storeConstantByGroupIndex(int index);
    void storeAllConstant();
    void storeRuntimeDataByGroupIndex(int index);
    void storeAllRuntimeData();

    void importConstantByGroupIndex(int index);
    void importAllConstant();
    void importRuntimeDataByGroupIndex(int index);
    void importAllRuntimeData();

    void handleTaskCompletedSlot(QString taskName, QVariantList *args);

private:
    explicit PageDataGenerator(QObject *parent = nullptr);
};

#endif // PAGEDATAGENERATOR_H
