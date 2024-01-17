#include "model/pagedatagenerator.h"
#include "model/fizeaudatagenerator.h"
#include "model/frequencedatagenerator.h"
#include "model/laserdatagenerator.h"
#include "model/spectrumdatagenerator.h"

PageDataGenerator *PageDataGenerator::pageDataGeneratorInstance = nullptr;

PageDataGenerator::PageDataGenerator(QObject *parent)
    : QObject(parent)
{}

PageDataGenerator *PageDataGenerator::getPageDataGeneratorInstance()
{
    if (pageDataGeneratorInstance == nullptr) {
        pageDataGeneratorInstance = new PageDataGenerator();
    }
    return pageDataGeneratorInstance;
}

void PageDataGenerator::destroyPageDataGeneratorInstance()
{
    if (pageDataGeneratorInstance != nullptr) {
        delete pageDataGeneratorInstance;
        pageDataGeneratorInstance = nullptr;
    }
}

PageDataGenerator::~PageDataGenerator() {}

QVector<double> *PageDataGenerator::generateData(DataType dataType,
                                                 const InputDataListManager *inputDataList)
{
    QVector<double> *data = nullptr;
    switch (dataType) {
    case DataType::Frequence:
        data = FrequenceDataGenerator::generateFrequenceData(inputDataList);
        break;
    case DataType::Laser:
        data = LaserDataGenerator::generateLaserData(inputDataList);
        break;
    case DataType::MieScattering:
        data = SpectrumDataGenerator::generateMieScatteringData(inputDataList);
        break;
    case DataType::BriScattering:
        data = SpectrumDataGenerator::generateBriScatteringData(inputDataList);
        break;
    case DataType::RayScattering:
        data = SpectrumDataGenerator::generateRayScatteringData(inputDataList);
        break;
    // case DataType::UnderwaterScattering:
    //     UnderwaterScatteringDataGenerator::generateUnderwaterScatteringData(data);
    //     break;
    case DataType::FizeauInstrument:
        data = FizeauDataGenerator::generateFizeauData(inputDataList);
        break;
    case DataType::FizeauSpectra:
        data = FizeauDataGenerator::generateSpectrumAfterFizeau(inputDataList);
        break;
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

void PageDataGenerator::generatePairOfData(int page_index, const InputDataListManager *inputDataList)
{
    QVector<QVector<double> *> *xDataVector;
    QVector<QVector<double> *> *yDataVector;
    xDataVector = new QVector<QVector<double> *>;
    yDataVector = new QVector<QVector<double> *>;

    switch (page_index) {
    case 1:
        xDataVector->append(generateData(DataType::Frequence, inputDataList));
        yDataVector->append(generateData(DataType::Laser, inputDataList));
        emit dataGenerated(xDataVector, yDataVector, 1);
        break;
    case 2:
        xDataVector->append(generateData(DataType::Frequence, inputDataList));
        yDataVector->append(generateData(DataType::MieScattering, inputDataList));
        yDataVector->append(generateData(DataType::BriScattering, inputDataList));
        yDataVector->append(generateData(DataType::RayScattering, inputDataList));
        emit dataGenerated(xDataVector, yDataVector, 3);
        break;
        // case 3:
        //     xDataVector->append(
        //         PageDataGenerator::generateData(PageDataGenerator::DataType::Frequence, inputDataList));
        //     yDataVector->append(
        //         PageDataGenerator::generateData(PageDataGenerator::DataType::MieScattering,
        //                                         inputDataList));
    case 4:
        xDataVector->append(generateData(DataType::Frequence, inputDataList));
        yDataVector->append(generateData(DataType::FizeauInstrument, inputDataList));
        yDataVector->append(generateData(DataType::FizeauSpectra, inputDataList));
        emit dataGenerated(xDataVector, yDataVector, 2);
        break;
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
    delete inputDataList;
}
