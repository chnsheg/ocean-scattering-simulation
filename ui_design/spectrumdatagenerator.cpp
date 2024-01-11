#include "spectrumdatagenerator.h"
#include "readfiledata.h"

SpectrumDataGenerator::SpectrumDataGenerator() {}

void SpectrumDataGenerator::generateBriScatteringData(QVector<double> *data)
{
    // Generate spectrum data
    // Assign x and y data accordingly
    data->clear();
    ReadFileData::readCSV(data, "L_bc.csv");
}

void SpectrumDataGenerator::generateMieScatteringData(QVector<double> *data)
{
    // Generate spectrum data
    // Assign x and y data accordingly
    data->clear();
    ReadFileData::readCSV(data, "L_mc.csv");
}

void SpectrumDataGenerator::generateRayScatteringData(QVector<double> *data)
{
    // Generate spectrum data
    // Assign x and y data accordingly
    data->clear();
    ReadFileData::readCSV(data, "L_rc.csv");
}
