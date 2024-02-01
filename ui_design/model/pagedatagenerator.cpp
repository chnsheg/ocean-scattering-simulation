#include "model/pagedatagenerator.h"
#include "model/fizeaudatagenerator.h"
#include "model/frequencedatagenerator.h"
#include "model/laserdatagenerator.h"
#include "model/spectrumdatagenerator.h"
#include "model/constantstorage.h"
#include "model/constantmap.h"

PageDataGenerator::PageDataGenerator(QObject *parent)
    : QObject(parent)
{
    Singleton<ConstantStorage>::getInstance(nullptr);
    // 注册元类型
    qRegisterMetaType<QSharedPointer<QCPGraphDataContainer>>("QSharedPointer<QCPGraphDataContainer>");
}

PageDataGenerator::~PageDataGenerator() {}

QVector<double> *PageDataGenerator::generateData(DataType dataType)
{
    QVector<double> *data = nullptr;
    switch (dataType)
    {
    case DataType::Frequence:
        data = FrequenceDataGenerator::generateFrequenceData();
        break;
    case DataType::Laser:
        data = LaserDataGenerator::generateLaserData();
        break;
    // case DataType::MieScattering:
    //     data = SpectrumDataGenerator::generateMieScatteringData();
    //     break;
    // case DataType::BriScattering:
    //     data = SpectrumDataGenerator::generateBriScatteringData();
    //     break;
    // case DataType::RayScattering:
    //     data = SpectrumDataGenerator::generateRayScatteringData();
    //     break;
    // case DataType::UnderwaterScattering:
    //     UnderwaterScatteringDataGenerator::generateUnderwaterScatteringData(data);
    //     break;
    // case DataType::FizeauInstrument:
    //     data = FizeauDataGenerator::generateFizeauData();
    //     break;
    // case DataType::FizeauSpectra:
    //     data = FizeauDataGenerator::generateSpectrumAfterFizeau();
    //     break;
    // case DataType::PMTFrequency:
    //     PMTFrequencyDataGenerator::generatePMTFrequencyData(data);
    //     break;
    // case DataType::PMTEnergy:
    //     PMTEnergyDataGenerator::generatePMTEnergyData(data);
    //     break;
    // Add more cases for other page types
    default:
        data = nullptr;
        break;
    }

    return data;
}

void PageDataGenerator::generatePairOfData(int page_index)
{
    QVector<QVector<double> *> *xDataVector;
    QVector<QVector<double> *> *yDataVector;
    xDataVector = new QVector<QVector<double> *>;
    yDataVector = new QVector<QVector<double> *>;

    switch (page_index)
    {
    case 1:
        xDataVector->append(generateData(DataType::Frequence));
        yDataVector->append(generateData(DataType::Laser));
        emit dataGenerated(xDataVector, yDataVector, 1);
        break;
        // case 2:
        //     xDataVector->append(generateData(DataType::Frequence));
        //     yDataVector->append(generateData(DataType::MieScattering));
        //     yDataVector->append(generateData(DataType::BriScattering));
        //     yDataVector->append(generateData(DataType::RayScattering));
        //     emit dataGenerated(xDataVector, yDataVector, 3);
        //     break;
        // case 3:
        //     xDataVector->append(
        //         PageDataGenerator::generateData(PageDataGenerator::DataType::Frequence, inputDataList));
        //     yDataVector->append(
        //         PageDataGenerator::generateData(PageDataGenerator::DataType::MieScattering,
        //                                         inputDataList));
        // case 4:
        //     xDataVector->append(generateData(DataType::Frequence));
        //     yDataVector->append(generateData(DataType::FizeauInstrument));
        //     yDataVector->append(generateData(DataType::FizeauSpectra));
        //     emit dataGenerated(xDataVector, yDataVector, 2);
        //     break;
        // case 5:
        //     xDataVector->append(
        //         PageDataGenerator::generateData(PageDataGenerator::DataType::Frequence, inputDataList));
        //     yDataVector->append(
        //         PageDataGenerator::generateData(PageDataGenerator::DataType::UnderwaterScattering,
        //                                         inputDataList));
        //     emit dataGenerated(xDataVector, yDataVector, 1);
        //     break;
        // case 6:
        //     xDataVector->append(
        //         PageDataGenerator::generateData(PageDataGenerator::DataType::PMTFrequency,
        //                                         inputDataList));
        //     yDataVector->append(
        //         PageDataGenerator::generateData(PageDataGenerator::DataType::PMTEnergy,
        //                                         inputDataList));
        //     emit dataGenerated(xDataVector, yDataVector, 1);
        //     break;
    }
    // 释放InputDataListManager内存
    // delete inputDataList;
}

// void PageDataGenerator::parseInputDataList(const InputDataListManager *inputDataList)
// {
//     for (int i = 0; i < inputDataList->getInputDataList()->size(); i++)
//     {
//         qDebug() << inputDataList->getInputDataList()->at(i).getInputDataName();
//         qDebug() << inputDataList->getInputDataList()->at(i).getInputDataValue();
//     }
// }

void PageDataGenerator::storeRuntimeDataByIndex(QSharedPointer<QCPGraphDataContainer> dataContainer, const int page_index, const int curve_index)
{
    switch (page_index)
    {
    case 1:
        Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(5, curve_index), QVariant::fromValue(dataContainer));
        // 打印底层存储地址
        // qDebug() << "dataContainer: " << dataContainer.data();
        break;
        // case 2:
        //     MieScatteringDataGenerator::saveRuntimeData();
        //     BriScatteringDataGenerator::saveRuntimeData();
        //     RayScatteringDataGenerator::saveRuntimeData();
        //     break;
        // case 3:
        //     MieScatteringDataGenerator::saveRuntimeData();
        //     break;
        // case 4:
        //     FizeauDataGenerator::saveRuntimeData();
        //     break;
        // case 5:
        //     UnderwaterScatteringDataGenerator::saveRuntimeData();
        //     break;
        // case 6:
        //     PMTFrequencyDataGenerator::saveRuntimeData();
        //     PMTEnergyDataGenerator::saveRuntimeData();
        //     break;
    }
}