#include "pmtreceptiondatagenerator.h"
#include "model/frequencedatagenerator.h"
#include "matlab/PMTReceive.h"

PMTReceptionDataGenerator::PMTReceptionDataGenerator() {}

QVector<QVector<double> *> *PMTReceptionDataGenerator::generatePMTReceptionData()
{
    // void PMTReceive(const coder::array<double, 2U> &frequency,
    //             const coder::array<double, 2U> &InputSpectrum,
    //             double NumberChannels, double channel_width,
    //             double channel_space, coder::array<double, 2U> &channel_energy,
    //             coder::array<double, 2U> &channel_sign)
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);
    double NumberChannels = constantStorage->getConstant(constantMap->getConstantName(3, 0)).toDouble();
    double channel_width = constantStorage->getConstant(constantMap->getConstantName(3, 1)).toDouble();
    double channel_space = constantStorage->getConstant(constantMap->getConstantName(3, 2)).toDouble();

    coder::array<double, 2U> frequency;
    coder::array<double, 2U> InputSpectrum;
    coder::array<double, 2U> channel_energy;
    coder::array<double, 2U> channel_sign;

    QVector<double> *RF = FrequenceDataGenerator::generateRelativeFrequenceData();
    QSharedPointer<QCPGraphDataContainer> dataContainer;
    //  constantStorage->getConstant(constantMap->getConstantName(5, 11))
    dataContainer = constantStorage->getConstant(constantMap->getConstantName(5, 13)).value<QSharedPointer<QCPGraphDataContainer>>();

    if (dataContainer.isNull())
    {
        Singleton<Logger>::getInstance()->logMessage("菲涅尔干涉仪数据为空！请先生成菲涅尔干涉仪数据！", Logger::Warning);
        return nullptr;
    }

    QVector<double> *xData = new QVector<double>();
    QVector<double> *yData = new QVector<double>();
    constantStorage->convertQSharedPointerToQVector(dataContainer, xData, yData);

    MyMath::convertQVectorToArray(RF, frequency);
    MyMath::convertQVectorToArray(yData, InputSpectrum);

    PMTReceive(frequency, InputSpectrum, NumberChannels, channel_width, channel_space, channel_energy, channel_sign);

    QVector<QVector<double> *> *result = new QVector<QVector<double> *>();
    QVector<double> *energy_vector = new QVector<double>();
    QVector<double> *sign_vector = new QVector<double>();

    energy_vector = MyMath::convertArrayToQVector(channel_energy);
    sign_vector = MyMath::convertArrayToQVector(channel_sign);

    result->append(RF);
    result->append(yData);
    result->append(sign_vector);
    result->append(energy_vector);

    // delete RF;
    delete xData;
    // delete yData;

    return result;
}

QVector<QVector<double> *> *PMTReceptionDataGenerator::receiveSpectrumAfterPMT(QVector<double> *spectrum)
{
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);
    // double NumberChannels = constantStorage->getConstant(constantMap->getConstantName(3, 0)).toDouble();
    // double channel_width = constantStorage->getConstant(constantMap->getConstantName(3, 1)).toDouble();
    // double channel_space = constantStorage->getConstant(constantMap->getConstantName(3, 2)).toDouble();

    coder::array<double, 2U> frequency;
    coder::array<double, 2U> InputSpectrum;
    coder::array<double, 2U> channel_energy;
    coder::array<double, 2U> channel_sign;

    QVector<double> *RF = FrequenceDataGenerator::generateRelativeFrequenceData();

    MyMath::convertQVectorToArray(RF, frequency);
    MyMath::convertQVectorToArray(spectrum, InputSpectrum);

    QVector<QVector<double> *> *result = new QVector<QVector<double> *>();
    QVector<double> *energy_vector = new QVector<double>();
    QVector<double> *sign_vector = new QVector<double>();

    PMTReceive(frequency, InputSpectrum, 16, 0.8e-3, 0.2e-3, channel_energy, channel_sign);

    energy_vector = MyMath::convertArrayToQVector(channel_energy);
    sign_vector = MyMath::convertArrayToQVector(channel_sign);

    result->append(sign_vector);
    result->append(energy_vector);

    for (int i = 0; i < result->at(0)->size(); ++i)
    {
        qDebug() << "sign: " << result->at(0)->at(i) << "energy: " << result->at(1)->at(i) << Qt::endl;
    }

    delete RF;

    return result;
}