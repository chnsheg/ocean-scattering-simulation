#include "laserdatagenerator.h"
#include "readfiledata.h"
LaserDataGenerator::LaserDataGenerator() {}

void LaserDataGenerator::generateLaserData(QVector<double> *data)
{
    // Generate Laser data
    // Assign x and y data accordingly
    data->clear();
    ReadFileData::readCSV(data, "laser_data.csv");
}
