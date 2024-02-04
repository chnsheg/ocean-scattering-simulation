#include "model/frequencedatagenerator.h"
#include <QDebug>
#include "utils/readfiledata.h"
#include "QMath.h"
#include "model/constantstorage.h"
#include "model/constantmap.h"
#include "singleton.h"

QVector<double> *FrequenceDataGenerator::data = nullptr;
QVector<double> *FrequenceDataGenerator::relativeData = nullptr;

QVector<double> *FrequenceDataGenerator::generateFrequenceData()
{
    if (data == nullptr || data->isEmpty() == true)
    {
        double lambda_0 = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 1)).toDouble();
        double c = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 4)).toDouble();
        double frequencyRange = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 3)).toDouble();

        double step = 1e6;
        double miu_0 = c / lambda_0;
        double lowerLimit = miu_0 - frequencyRange;
        double upperLimit = miu_0 + frequencyRange;
        int size = (upperLimit - lowerLimit) / step + 1;

        data = new QVector<double>(size);

        for (int i = 0; i < size; i++)
        {
            (*data)[i] = lowerLimit + i * step;
        }

        // frequence_count += 1;
    }
    return data;
}

QVector<double> *FrequenceDataGenerator::generateRelativeFrequenceData()
{

    if (relativeData != nullptr && relativeData->isEmpty() == false)
    {
        return relativeData;
    }

    if (data == nullptr || data->isEmpty() == true)
    {
        generateFrequenceData();
    }

    if (data != nullptr && data->isEmpty() == false)
    {
        relativeData = new QVector<double>(*data);
        double miu_0 = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 4)).toDouble() / Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 1)).toDouble();
        for (int i = 0; i < relativeData->size(); i++)
        {
            (*relativeData)[i] = ((*relativeData)[i] - miu_0) / 1e9;
        }
        // relative_frequence_count += 1;
    }

    return relativeData;
}
