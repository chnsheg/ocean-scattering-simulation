#include "frequencedatagenerator.h"
#include "readfiledata.h"

void FrequenceDataGenerator::generateFrequenceData(QVector<double> *data)
{
    // Generate RF data
    // Assign x and y data accordingly
    data->clear();
    ReadFileData::readCSV(data, "RF.csv");
}
