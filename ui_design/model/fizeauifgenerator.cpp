#include "fizeauifgenerator.h"
#include "utils/logger.h"
#include "matlab/FizeauInstrument.h"
#include "model/constantstorage.h"
#include "model/constantmap.h"
#include "model/frequencedatagenerator.h"
#include "utils/mymath.h"

FizeauIFGenerator::FizeauIFGenerator() {}

QVector<QVector<double> *> *FizeauIFGenerator::generateFizeauIFData()
{
    Singleton<Logger>::getInstance()->logMessage("生成菲涅尔干涉仪数据", Logger::Log);

    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);

    double lambda = constantStorage->getConstant(constantMap->getConstantName(2, 0)).toDouble();
    double Fizeau_plate = constantStorage->getConstant(constantMap->getConstantName(2, 1)).toDouble();
    double Fizeau_angle = constantStorage->getConstant(constantMap->getConstantName(2, 2)).toDouble();
    double Fizeau_index = constantStorage->getConstant(constantMap->getConstantName(2, 3)).toDouble();
    double Fizeau_albedo = constantStorage->getConstant(constantMap->getConstantName(2, 4)).toDouble();

    double channel_number = constantStorage->getConstant(constantMap->getConstantName(3, 0)).toDouble();
    double channel_width = constantStorage->getConstant(constantMap->getConstantName(3, 1)).toDouble();
    double channel_space = constantStorage->getConstant(constantMap->getConstantName(3, 2)).toDouble();

    double PMT_length = channel_number * channel_width + (channel_number - 1) * channel_space;
    double alpha_cal = atan(lambda / 2 / PMT_length);

    double *Fizeau_FSR = new double();
    coder::array<double, 2U> Fizeau_spectrum;
    QVector<double> *RF = FrequenceDataGenerator::generateRelativeFrequenceData();
    coder::array<double, 2U> Fizeau_frequency;
    coder::array<double, 2U> frequency;

    MyMath::convertQVectorToArray(RF, frequency);

    if (Fizeau_angle > alpha_cal)
    {
        Singleton<Logger>::getInstance()->logMessage("菲涅尔干涉仪角度大于计算角度，无法计算", Logger::Warning);
        return nullptr;
    }

    //[FSR, FizeauFunction, ~] = FizeauInstrument(alpha_cal, Frequency, lambda, Fizeau_plate, Fizeau_angle, Fizeau_index, Fizeau_albedo);
    FizeauInstrument(alpha_cal, frequency, lambda, Fizeau_plate, Fizeau_angle, Fizeau_index, Fizeau_albedo, Fizeau_FSR, Fizeau_spectrum, Fizeau_frequency);

    Singleton<Logger>::getInstance()->logMessage("菲涅尔干涉仪数据生成完成", Logger::Log);
    // 打印FSR
    Singleton<Logger>::getInstance()->logMessage("FSR: " + QString::number(*Fizeau_FSR), Logger::Log);

    // 将Fizeau_spectrum和Fizeau_frequency转换为QVector<QVector<double> *>
    QVector<QVector<double> *> *result = new QVector<QVector<double> *>();
    QVector<double> *spectrum_vector = new QVector<double>();
    QVector<double> *frequency_vector = new QVector<double>();

    spectrum_vector = MyMath::convertArrayToQVector(Fizeau_spectrum);
    frequency_vector = MyMath::convertArrayToQVector(Fizeau_frequency);

    // Fizeau_spectrum = FizeauFunction ./ polyarea(Frequency, FizeauFunction); %Area normalization
    // double area = MyMath::polyarea(*frequency_vector, *spectrum_vector);
    double area = MyMath::polyarea(*RF, *spectrum_vector);
    for (int i = 0; i < Fizeau_spectrum.size(1); ++i)
    {
        // spectrum_vector[i] = spectrum_vector[i] / area;
        (*spectrum_vector)[i] = (*spectrum_vector)[i] / area;
    }

    result->append(frequency_vector);
    result->append(spectrum_vector);

    delete RF;

    return result;
}

QVector<QVector<double> *> *FizeauIFGenerator::calculateSpectrumAfterFizeau(QVector<double> *fizeau_IF)
{
    Singleton<Logger>::getInstance()->logMessage("计算通过菲涅尔干涉仪后的光谱", Logger::Log);
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);

    QSharedPointer<QCPGraphDataContainer> dataContainer;
    //  constantStorage->getConstant(constantMap->getConstantName(5, 11))
    dataContainer = constantStorage->getConstant(constantMap->getConstantName(5, 11)).value<QSharedPointer<QCPGraphDataContainer>>();

    if (dataContainer.isNull())
    {
        Singleton<Logger>::getInstance()->logMessage("菲涅尔干涉仪数据为空！请先生成菲涅尔干涉仪数据！", Logger::Warning);
        return nullptr;
    }

    QVector<double> *xData = new QVector<double>();
    QVector<double> *yData = new QVector<double>();
    constantStorage->convertQSharedPointerToQVector(dataContainer, xData, yData);

    QVector<double> *result = MyMath::convolution(yData, fizeau_IF);

    QVector<QVector<double> *> *resultVector = new QVector<QVector<double> *>();
    resultVector->append(xData);
    resultVector->append(result);

    delete yData;

    return resultVector;
}