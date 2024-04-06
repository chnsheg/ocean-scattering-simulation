#include "pmtreceptiondatagenerator.h"
#include "model/frequencedatagenerator.h"
#include "matlab/PMTReceive.h"
#include "matlab/Retrieval/RetrievalAlgorithm.h"
#include "matlab/Retrieval/Retrieval_TS.h"

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

    // 存储energy_vector指向的内存地址
    constantStorage->setConstant(constantMap->getConstantName(5, 14), QVariant::fromValue(new QVector<double>(*energy_vector)));

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
    // QVector<double> *PMT_energy_vector = new QVector<double>(
    //     {0.000500951818447207, 0.00126892042344298, 0.00418905457824441, 0.00363221019365598, 0.00110851062510006, 0.000566371696804416, 0.000442973766125158, 0.000638886278194898, 0.000629230476113756, 0.000443742611150079, 0.000577954102157679, 0.00114477405738897, 0.00373425200890559, 0.00409628632197294, 0.00121853330725838, 0.000488091198032589});

    QVector<double> *PMT_energy_vector = constantStorage->getConstant(constantMap->getConstantName(5, 14)).value<QVector<double> *>();

    for (int i = 0; i < PMT_energy_vector->size(); ++i)
    {
        qDebug() << "PMT_energy_vector[" << i << "]: " << PMT_energy_vector->at(i) << Qt::endl;
    }

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
    // double params[19] = {532e-9, 12e9, 1, 100e6, 20e-3, 0.08, 0.00, 2.4e-4, 0.00, 1.3333, 10, 150, 0.04, 0.05, 1, 10, 2, 0.13, 0.4};

    double wave_length = constantStorage->getConstant(constantMap->getConstantName(0, 1)).toDouble();
    double freq_range = constantStorage->getConstant(constantMap->getConstantName(0, 3)).toDouble();
    double intensity = constantStorage->getConstant(constantMap->getConstantName(0, 2)).toDouble();
    double laser_width = constantStorage->getConstant(constantMap->getConstantName(0, 0)).toDouble();
    double laser_energy = constantStorage->getConstant(constantMap->getConstantName(0, 5)).toDouble();
    double alpha = constantStorage->getConstant(constantMap->getConstantName(7, 1)).toDouble();
    double beta_p = constantStorage->getConstant(constantMap->getConstantName(1, 2)).toDouble(); // 有一个参数没用到
    double beta_m = constantStorage->getConstant(constantMap->getConstantName(1, 3)).toDouble();
    double n = constantStorage->getConstant(constantMap->getConstantName(1, 4)).toDouble();
    double z = constantStorage->getConstant(constantMap->getConstantName(7, 6)).toDouble();
    double H = constantStorage->getConstant(constantMap->getConstantName(7, 7)).toDouble();
    double r = constantStorage->getConstant(constantMap->getConstantName(7, 4)).toDouble();
    double M = constantStorage->getConstant(constantMap->getConstantName(7, 0)).toDouble();
    double N_dark = constantStorage->getConstant(constantMap->getConstantName(7, 5)).toDouble();
    double beta = constantStorage->getConstant(constantMap->getConstantName(7, 8)).toDouble();
    double xi = constantStorage->getConstant(constantMap->getConstantName(7, 2)).toDouble();
    double xi_f = constantStorage->getConstant(constantMap->getConstantName(7, 3)).toDouble();
    double energy_ratio = constantStorage->getConstant(constantMap->getConstantName(1, 5)).toDouble();

    double params[19] = {wave_length, freq_range, intensity, laser_width, laser_energy, alpha, beta_p, beta_m, beta_p, n, z, H, energy_ratio, r, M, N_dark, beta, xi, xi_f};

    double fitted_value[4];
    double resnorm;
    double exitflag;
    struct0_T output;
    struct1_T lambda;

    RetrievalAlgorithm(number, PMT_energy, Fizeau_spectrum, Initial_lower, Initial_upper, Initial_value, params, fitted_value, &resnorm, residual, &exitflag, &output, &lambda, jacobia);

    Singleton<Logger>::getInstance()->logMessage("PMT接收数据处理完成！", Logger::Info);

    //% 处理结果：
    // res.B_shift = fitted_value(1);
    // res.B_width = fitted_value(2);
    // res.R_width = fitted_value(3);
    // res.N_photo = fitted_value(4);

    double res_B_shift = fitted_value[0];
    double res_B_width = fitted_value[1];
    double res_R_width = fitted_value[2];
    // double res_N_photo = fitted_value[3];

    // % Original Parameters
    // EnvironmentalFactors.Bri_Shift = Bri_shift;
    // EnvironmentalFactors.Bri_Width = Bri_width;
    // EnvironmentalFactors.Ray_Width = 0.15e9;
    // EnvironmentalFactors.Num_Photo = PhotonNum;
    // EnvironmentalFactors.tem = Water_Temperature;
    // EnvironmentalFactors.sal = Water_Salinity;

    double Bri_shift = constantStorage->getConstant(constantMap->getConstantName(6, 1)).toDouble() * 1e9;
    double Bri_width = constantStorage->getConstant(constantMap->getConstantName(6, 0)).toDouble() * 1e9;
    double Ray_Width = 0.15e9;
    // double PhotonNum = constantStorage->getConstant(constantMap->getConstantName(6, 6)).toDouble();
    double Water_Temperature = constantStorage->getConstant(constantMap->getConstantName(1, 0)).toDouble();
    double Water_Salinity = constantStorage->getConstant(constantMap->getConstantName(1, 1)).toDouble();

    // % 反演环境要素：
    // [REF.Tem, REF.Sal] = Retrieval_TS(res.B_shift, res.B_width);

    double REF_Tem;
    double REF_Sal;
    Retrieval_TS(res_B_shift, res_B_width, &REF_Tem, &REF_Sal);

    // % 计算误差：
    // Error.shift = res.B_shift - EnvironmentalFactors.Bri_Shift;
    // Error.width = res.B_width - EnvironmentalFactors.Bri_Width;
    // Error.Rwidth = res.R_width - EnvironmentalFactors.Ray_Width;
    // Error.Photon = res.N_photo - EnvironmentalFactors.Num_Photo;

    double Error_shift = res_B_shift - Bri_shift;
    double Error_width = res_B_width - Bri_width;
    double Error_Rwidth = res_R_width - Ray_Width;
    // double Error_Photon = res_N_photo - PhotonNum;

    // Error.tem = REF.Tem - EnvironmentalFactors.tem;
    // Error.sal = REF.Sal - EnvironmentalFactors.sal;

    double Error_tem = REF_Tem - Water_Temperature;
    double Error_sal = REF_Sal - Water_Salinity;

    //     % 显示结果：
    // fprintf('Result')
    // fprintf('------------------------------------------------------------------------- \n');
    // fprintf('Spectral parameters|Measured(GHz)|Theoretical(GHz)|error(GHz) \n');
    // fprintf(' Brillouin width   %8.4f  %8.4f  %8.4f \n', res.B_width / 1e9, EnvironmentalFactors.Bri_Width / 1e9, Error.width / 1e9);
    // fprintf(' Brillouin shift   %8.4f  %8.4f  %8.4f \n', res.B_shift / 1e9, EnvironmentalFactors.Bri_Shift / 1e9, Error.shift / 1e9);
    // fprintf(' RAyleigh width    %8.4f  %8.4f  %8.4f \n', res.R_width / 1e9, EnvironmentalFactors.Ray_Width / 1e9, Error.Rwidth / 1e9);
    // fprintf(' Photon Number     %8.4e  %8.4e  %8.4e \n', res.N_photo, EnvironmentalFactors.Num_Photo, Error.Photon);

    // 打印结果
    qDebug() << "Result";
    qDebug() << "-------------------------------------------------------------------------";
    qDebug() << "Spectral parameters|Measured(GHz)|Theoretical(GHz)|error(GHz)";
    qDebug() << QString(" Brillouin width   %1    %2    %3")
                    .arg(res_B_width / 1e9, 8, 'f', 4)
                    .arg(Bri_width / 1e9, 8, 'f', 4)
                    .arg(Error_width / 1e9, 8, 'f', 4);
    qDebug() << QString(" Brillouin shift   %1  %2  %3")
                    .arg(res_B_shift / 1e9, 8, 'f', 4)
                    .arg(Bri_shift / 1e9, 8, 'f', 4)
                    .arg(Error_shift / 1e9, 8, 'f', 4);
    qDebug() << QString(" RAyleigh width    %1  %2  %3")
                    .arg(res_R_width / 1e9, 8, 'f', 4)
                    .arg(Ray_Width / 1e9, 8, 'f', 4)
                    .arg(Error_Rwidth / 1e9, 8, 'f', 4);
    // fprintf(stdout, " Photon Number     %8.4e  %8.4e  %8.4e \n", res_N_photo, PhotonNum, Error_Photon);

    // fprintf('------------------------------------------------------------------------- \n');
    // fprintf('Parameters       |   Measured  | Theoretical | error (%4.2f m: %4.2f)| \n', System_Depth, N_SNR);
    // fprintf('Environmental tem| %8.4f°C | %8.4f°C | %8.4f°C | \n', REF.Tem, EnvironmentalFactors.tem, Error.tem);
    // fprintf('Environmental Sal| %8.4f ‰ | %8.4f ‰ | %8.4f‰  | \n', REF.Sal, EnvironmentalFactors.sal, Error.sal);
    // fprintf('-------------------------------------------------------------------------\n');

    // fprintf(stdout, "------------------------------------------------------------------------- \n");
    // fprintf(stdout, "Parameters       |   Measured  | Theoretical | error (%4.2f m: %4.2f)| \n", 0.0, 0.0);
    // fprintf(stdout, "Environmental tem| %8.4f ℃ | %8.4f ℃  | %8.4f ℃ | \n", REF_Tem, Water_Temperature, Error_tem);
    // fprintf(stdout, "Environmental Sal| %8.4f ‰ | %8.4f ‰ | %8.4f ‰  | \n", REF_Sal, Water_Salinity, Error_sal);
    // fprintf(stdout, "-------------------------------------------------------------------------\n");

    qDebug() << "-------------------------------------------------------------------------";
    qDebug() << "Parameters       |   Measured  | Theoretical | error (%4.2f m: %4.2f)|";
    qDebug() << QString("Environmental tem| %1 °C | %2 °C | %3 °C |")
                    .arg(REF_Tem, 8, 'f', 4)
                    .arg(Water_Temperature, 8, 'f', 4)
                    .arg(Error_tem, 8, 'f', 4);
    qDebug() << QString("Environmental Sal| %1 ‰ | %2 ‰  | %3 ‰ |")
                    .arg(REF_Sal, 8, 'f', 4)
                    .arg(Water_Salinity, 8, 'f', 4)
                    .arg(Error_sal, 8, 'f', 4);
    qDebug() << "-------------------------------------------------------------------------";

    // 输出fitted_value
    for (int i = 0; i < 4; ++i)
    {
        // Singleton<Logger>::getInstance()->logMessage("fitted_value[" + QString::number(i) + "]: " + QString::number(fitted_value[i] / 1e9, Logger::Info);
        Singleton<Logger>::getInstance()->logMessage("fitted_value[" + QString::number(i) + "]: " + QString::number(fitted_value[i] / 1e9), Logger::Info);
    }

    delete xData;
    delete yData;
    // delete PMT_energy_vector;
}