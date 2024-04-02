#include "pmtreceptiondatagenerator.h"
#include "model/frequencedatagenerator.h"
#include "matlab/PMTReceive.h"
#include "matlab/Retrieval/RetrievalAlgorithm.h"

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

void PMTReceptionDataGenerator::retrievalFormPMT()
{
    // void RetrievalAlgorithm(double number,
    //                     const coder::array<double, 2U> &PMT_energy,
    //                     const coder::array<double, 2U> &Fizeau_spectrum,
    //                     const double Initial_lower[4],
    //                     const double Initial_upper[4],
    //                     const double Initial_value[4], const double params[19],
    //                     double fitted_value[4], double *resnorm,
    //                     coder::array<double, 2U> &residual, double *exitflag,
    //                     struct0_T *output, struct1_T *lambda,
    //                     coder::array<double, 2U> &jacobia)

    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);

    coder::array<double, 2U> PMT_energy;
    coder::array<double, 2U> Fizeau_spectrum;
    coder::array<double, 2U> residual;
    coder::array<double, 2U> jacobia;

    double number = constantStorage->getConstant(constantMap->getConstantName(3, 0)).toDouble();
    // QVector<double> *PMT_energy_vector = new QVector<double>();
    // 0.000500951818447207	0.00126892042344298	0.00418905457824441	0.00363221019365598	0.00110851062510006	0.000566371696804416	0.000442973766125158	0.000638886278194898	0.000629230476113756	0.000443742611150079	0.000577954102157679	0.00114477405738897	0.00373425200890559	0.00409628632197294	0.00121853330725838	0.000488091198032589
    QVector<double> *PMT_energy_vector = new QVector<double>(
        {0.000500951818447207, 0.00126892042344298, 0.00418905457824441, 0.00363221019365598, 0.00110851062510006, 0.000566371696804416, 0.000442973766125158, 0.000638886278194898, 0.000629230476113756, 0.000443742611150079, 0.000577954102157679, 0.00114477405738897, 0.00373425200890559, 0.00409628632197294, 0.00121853330725838, 0.000488091198032589});

    QSharedPointer<QCPGraphDataContainer> dataContainer;
    //  constantStorage->getConstant(constantMap->getConstantName(5, 11))
    dataContainer = constantStorage->getConstant(constantMap->getConstantName(5, 12)).value<QSharedPointer<QCPGraphDataContainer>>();

    if (dataContainer.isNull())
    {
        Singleton<Logger>::getInstance()->logMessage("请先生成Fizeau仪器函数！", Logger::Warning);
        return;
    }

    QVector<double> *xData = new QVector<double>();
    QVector<double> *yData = new QVector<double>();
    constantStorage->convertQSharedPointerToQVector(dataContainer, xData, yData);

    MyMath::convertQVectorToArray(PMT_energy_vector, PMT_energy);
    MyMath::convertQVectorToArray(yData, Fizeau_spectrum);

    double Initial_lower[4] = {7.0e9, 0.5e9, 0.1e9, 0.03e9};
    double Initial_upper[4] = {7.7e9, 0.7e9, 0.2e9, 0.05e9};
    double Initial_value[4] = {7.6732e9, 0.617e9, 0.13e9, 0.03e9};

    // params = [532e-9, 12e9, 1, 100e6, 20e-3, 0.08, 0.00, 2.4e-4, 0.00, 1.3333, 10, 150, 0.04, 0.05, 1, 10, 2, 0.13, 0.4];
    double params[19] = {532e-9, 12e9, 1, 100e6, 20e-3, 0.08, 0.00, 2.4e-4, 0.00, 1.3333, 10, 150, 0.04, 0.05, 1, 10, 2, 0.13, 0.4};

    double fitted_value[4];
    double resnorm;
    double exitflag;
    struct0_T output;
    struct1_T lambda;

    RetrievalAlgorithm(number, PMT_energy, Fizeau_spectrum, Initial_lower, Initial_upper, Initial_value, params, fitted_value, &resnorm, residual, &exitflag, &output, &lambda, jacobia);

    Singleton<Logger>::getInstance()->logMessage("PMT接收数据处理完成！", Logger::Info);
    // 输出fitted_value
    for (int i = 0; i < 4; ++i)
    {
        Singleton<Logger>::getInstance()->logMessage("fitted_value[" + QString::number(i) + "]: " + QString::number(fitted_value[i]), Logger::Info);
    }

    delete xData;
    delete yData;
    delete PMT_energy_vector;
}