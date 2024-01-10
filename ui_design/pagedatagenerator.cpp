#include "pagedatagenerator.h"
#include "frequencedatagenerator.h"

void PageDataGenerator::generateData(DataType dataType, QVector<double> *data)
{
    switch (dataType) {
    case DataType::Frequence:
        FrequenceDataGenerator::generateFrequenceData(data);
        break;
    // case DataType::Laser:
    //     LaserDataGenerator::generateLaserData(data);
    //     break;
    // case DataType::MieScattering:
    //     MieScatteringDataGenerator::generateMieScatteringData(data);
    //     break;
    // case DataType::BriScattering:
    //     BriScatteringDataGenerator::generateBriScatteringData(data);
    //     break;
    // case DataType::RayScattering:
    //     RayScatteringDataGenerator::generateRayScatteringData(data);
    //     break;
    // case DataType::UnderwaterScattering:
    //     UnderwaterScatteringDataGenerator::generateUnderwaterScatteringData(data);
    //     break;
    // case DataType::FizeauInstrument:
    //     FizeauInstrumentDataGenerator::generateFizeauInstrumentData(data);
    //     break;
    // case DataType::FizeauSpectra:
    //     FizeauSpectraDataGenerator::generateFizeauSpectraData(data);
    //     break;
    // case DataType::PMTFrequency:
    //     PMTFrequencyDataGenerator::generatePMTFrequencyData(data);
    //     break;
    // case DataType::PMTEnergy:
    //     PMTEnergyDataGenerator::generatePMTEnergyData(data);
    //     break;
    // Add more cases for other page types
    default:
        break;
    }
}
