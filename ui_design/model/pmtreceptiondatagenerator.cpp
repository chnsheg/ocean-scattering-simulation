#include "pmtreceptiondatagenerator.h"
#include "model/frequencedatagenerator.h"
#include "matlab/PMTReceive.h"
#include "matlab/Retrieval/RetrievalAlgorithm.h"
#include "matlab/Retrieval/Retrieval_TS.h"
#include "../matlab/AddNoiseNondB.h"

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

    // 根据RF算出x横轴的最大距离
    int max_x = (int)(RF->at(RF->size() - 1) - RF->at(0));
    // 计算柱形图的宽度
    double bar_width = max_x / NumberChannels * channel_width / (channel_space + channel_width);

    QSharedPointer<QCPGraphDataContainer> dataContainer;
    //  constantStorage->getConstant(constantMap->getConstantName(5, 11))
    dataContainer = constantStorage->getConstant(constantMap->getConstantName(5, 13)).value<QSharedPointer<QCPGraphDataContainer>>();

    if (dataContainer.isNull())
    {
        Singleton<Logger>::getInstance()
            ->logMessage("Fizeau interferometer data is empty! Please provide data from the "
                         "Fresnel interferometer.",
                         Logger::Warning);
        return nullptr;
    }

    QVector<double> *xData = new QVector<double>();
    QVector<double> *yData = new QVector<double>();
    constantStorage->convertQSharedPointerToQVector(dataContainer, xData, yData);

    MyMath::convertQVectorToArray(RF, frequency);
    MyMath::convertQVectorToArray(yData, InputSpectrum);

    // 从存储中获取SNR
    double SNR = constantStorage->getConstant(constantMap->getConstantName(6, 5)).toDouble();
    // coder::array<double, 2U> Iv;
    coder::array<double, 2U> Iv_Noised;
    coder::array<double, 2U> Pmt_Noised;
    QVector<double> *Spectrum_Noised;
    // MyMath::convertQVectorToArray(energy_vector, Iv);
    double poissrnd_lambda = constantStorage->getConstant(constantMap->getConstantName(3, 3)).toDouble();

    QVector<double> *energy_vector;
    QVector<double> *sign_vector;
    QVector<QVector<double> *> *result = new QVector<QVector<double> *>();

    // 先不加噪接收一次
    PMTReceive(frequency, InputSpectrum, NumberChannels, channel_width, channel_space, channel_energy, channel_sign);
    energy_vector = MyMath::convertArrayToQVector(channel_energy);
    sign_vector = MyMath::convertArrayToQVector(channel_sign);
    // 对输入光谱进行加噪
    AddNoiseNondB(InputSpectrum, SNR, Iv_Noised, &SNR, poissrnd_lambda);

    // 用yData存储加噪后的光谱
    Spectrum_Noised = MyMath::convertArrayToQVector(Iv_Noised);
    delete yData;
    yData = Spectrum_Noised;

    PMTReceive(frequency, Iv_Noised, NumberChannels, channel_width, channel_space, Pmt_Noised, channel_sign);

    // // 从存储中获取SNR
    // double SNR = constantStorage->getConstant(constantMap->getConstantName(6, 5)).toDouble();
    // // coder::array<double, 2U> Iv;
    // coder::array<double, 2U> Iv_Noised;
    // QVector<double> *Spectrum_Noised;
    // // MyMath::convertQVectorToArray(energy_vector, Iv);
    // double poissrnd_lambda = constantStorage->getConstant(constantMap->getConstantName(3, 3)).toDouble();
    // AddNoiseNondB(channel_energy, SNR, Iv_Noised, &SNR, poissrnd_lambda);
    // AddNoiseNondB(channel_energy, SNR, Iv_Noised, &SNR, 0);
    Spectrum_Noised = MyMath::convertArrayToQVector(Pmt_Noised);
    // result->append(sign_vector);
    // result->append(Spectrum_Noised);
    // result->append(sign_vector);
    // result->append(energy_vector);

    // 给sign_vector末尾添加柱形图的宽度
    sign_vector->append(bar_width);

    // 存储energy_vector指向的内存地址
    constantStorage->setConstant(constantMap->getConstantName(5, 14), QVariant::fromValue(new QVector<double>(*Spectrum_Noised)));

    result->append(RF);
    result->append(yData);
    result->append(sign_vector);
    result->append(Spectrum_Noised);
    // 以拷贝的方式，复制存储一份sign_vector
    result->append(new QVector<double>(*sign_vector));
    result->append(energy_vector);

    // for (int i = 0; i < sign_vector->size() - 1; ++i)
    // {
    //     qDebug() << "加噪前"
    //              << "sign: " << sign_vector->at(i + 1) << "energy: " << energy_vector->at(i) << Qt::endl;
    //     qDebug() << "加噪后"
    //              << "sign: " << sign_vector->at(i + 1) << "energy: " << Spectrum_Noised->at(i) << Qt::endl;
    // }

    //     afterPMTData: 0.00600039
    // afterPMTData: 0.014514
    // afterPMTData: 0.0458723
    // afterPMTData: 0.0484146
    // afterPMTData: 0.015531
    // afterPMTData: 0.00774155
    // afterPMTData: 0.00583176
    // afterPMTData: 0.0080638
    // afterPMTData: 0.00796253
    // afterPMTData: 0.00584695
    // afterPMTData: 0.00790417
    // afterPMTData: 0.0161331
    // afterPMTData: 0.0494295
    // afterPMTData: 0.0447573
    // afterPMTData: 0.0139654
    // afterPMTData: 0.00582907
    // Spectrum_Noised->replace(0, 0.00600039);
    // Spectrum_Noised->replace(1, 0.014514);
    // Spectrum_Noised->replace(2, 0.0458723);
    // Spectrum_Noised->replace(3, 0.0484146);
    // Spectrum_Noised->replace(4, 0.015531);
    // Spectrum_Noised->replace(5, 0.00774155);
    // Spectrum_Noised->replace(6, 0.00583176);
    // Spectrum_Noised->replace(7, 0.0080638);
    // Spectrum_Noised->replace(8, 0.00796253);
    // Spectrum_Noised->replace(9, 0.00584695);
    // Spectrum_Noised->replace(10, 0.00790417);
    // Spectrum_Noised->replace(11, 0.0161331);
    // Spectrum_Noised->replace(12, 0.0494295);
    // Spectrum_Noised->replace(13, 0.0447573);
    // Spectrum_Noised->replace(14, 0.0139654);
    // Spectrum_Noised->replace(15, 0.00582907);

    // delete RF;
    delete xData;
    // delete energy_vector;
    // delete yData;

    return result;
}

QVector<double> *PMTReceptionDataGenerator::receiveSpectrumAfterPMT(QVector<double> *spectrum)
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

    // QVector<QVector<double> *> *result = new QVector<QVector<double> *>();
    QVector<double> *energy_vector;
    QVector<double> *sign_vector;

    double NumberChannels = constantStorage->getConstant(constantMap->getConstantName(3, 0)).toDouble();
    double channel_width = constantStorage->getConstant(constantMap->getConstantName(3, 1)).toDouble();
    double channel_space = constantStorage->getConstant(constantMap->getConstantName(3, 2)).toDouble();

    PMTReceive(frequency, InputSpectrum, NumberChannels, channel_width, channel_space, channel_energy, channel_sign);

    energy_vector = MyMath::convertArrayToQVector(channel_energy);
    sign_vector = MyMath::convertArrayToQVector(channel_sign);

    // for (int i = 0; i < sign_vector->size() - 1; ++i)
    // {
    //     // 打印能量
    //     qDebug() << "energy++" << energy_vector->at(i) << Qt::endl;
    // }

    // 从存储中获取SNR
    // double SNR = constantStorage->getConstant(constantMap->getConstantName(6, 5)).toDouble();
    // coder::array<double, 2U> Iv;
    // coder::array<double, 2U> Iv_Noised;
    // QVector<double> *Spectrum_Noised;
    // MyMath::convertQVectorToArray(energy_vector, Iv);
    // double poissrnd_lambda = constantStorage->getConstant(constantMap->getConstantName(3, 3)).toDouble();
    // AddNoiseNondB(channel_energy, SNR, Iv_Noised, &SNR, poissrnd_lambda);
    // AddNoiseNondB(channel_energy, SNR, Iv_Noised, &SNR, 0);
    // Spectrum_Noised = MyMath::convertArrayToQVector(Iv_Noised);
    // result->append(sign_vector);
    // result->append(energy_vector);
    // result->append(sign_vector);
    // result->append(energy_vector);

    // for (int i = 0; i < result->at(0)->size(); ++i)
    // {
    //     // 加噪前
    //     qDebug() << "sign: " << sign_vector->at(i) << "energy: " << energy_vector->at(i) << Qt::endl;
    //     // 加噪后
    //     qDebug() << "sign: " << result->at(0)->at(i) << "energy: " << result->at(1)->at(i) << Qt::endl;
    // }

    delete RF;
    delete sign_vector;
    delete spectrum;

    return energy_vector;
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
    //     {0.000481349812890324, 0.00115600087686415, 0.00366448196335308, 0.00403615874800880, 0.00130673405407780, 0.000646618950177882, 0.000485579648168674, 0.000661343988317603, 0.000652450122068401, 0.000477275657879331, 0.000645090618930230, 0.00133582055472497, 0.00410742981808581, 0.00357553374168214, 0.00111753722374564, 0.000471019177273276});

    QVector<double> *PMT_energy_vector = constantStorage->getConstant(constantMap->getConstantName(5, 14)).value<QVector<double> *>();

    if (PMT_energy_vector == nullptr || PMT_energy_vector->isEmpty())
    {
        Singleton<Logger>::getInstance()
            ->logMessage("PMT'Energy is empty! Please become PMT to receive data!", Logger::Warning);
        return;
    }

    // for (int i = 0; i < PMT_energy_vector->size(); ++i)
    // {
    //     // qDebug() << "PMT_energy_vector[" << i << "]: " << PMT_energy_vector->at(i) << Qt::endl;
    //     Singleton<Logger>::getInstance()->logMessage("PMT_energy_vector[" + QString::number(i) + "]: " + QString::number(PMT_energy_vector->at(i)), Logger::Info);
    // }

    QSharedPointer<QCPGraphDataContainer> dataContainer;
    //  constantStorage->getConstant(constantMap->getConstantName(5, 11))
    dataContainer = constantStorage->getConstant(constantMap->getConstantName(5, 12)).value<QSharedPointer<QCPGraphDataContainer>>();

    if (dataContainer.isNull())
    {
        Singleton<Logger>::getInstance()
            ->logMessage("Please generate the Fizeau instrument function!", Logger::Warning);
        return;
    }

    QVector<double> *xData = new QVector<double>();
    QVector<double> *yData = new QVector<double>();
    constantStorage->convertQSharedPointerToQVector(dataContainer, xData, yData);

    // 获取相对频率数据
    QVector<double> *RF = FrequenceDataGenerator::generateRelativeFrequenceData();

    // 先对Fizeau_spectrum进行面积归一化
    double area = MyMath::polyarea(*RF, *yData);
    for (int i = 0; i < yData->size(); ++i)
    {
        (*yData)[i] = (*yData)[i] / area;
    }

    delete RF;

    MyMath::convertQVectorToArray(PMT_energy_vector, PMT_energy);
    MyMath::convertQVectorToArray(yData, Fizeau_spectrum);

    // double Initial_lower[3] = {7.0e9, 0.2e9, 0.1e9};
    // double Initial_upper[3] = {8.3e9, 1e9, 0.3e9};
    // double Initial_value[3] = {7.6732e9, 0.617e9, 0.15e9};

    // double Initial_lower[3] = {7.2e9, 0.2e9, 0.1e9}; // 初始值越小，拟合越精确，耗时越短
    // double Initial_upper[3] = {7.8e9, 0.8e9, 0.3e9};
    // double Initial_value[3] = {7.6732e9, 0.617e9, 0.15e9};
    double Initial_lower[3] = {constantStorage->getConstant(constantMap->getConstantName(9, 6)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 7)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 8)).toDouble()};

    double Initial_upper[3] = {constantStorage->getConstant(constantMap->getConstantName(9, 3)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 4)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 5)).toDouble()};

    double Initial_value[3] = {constantStorage->getConstant(constantMap->getConstantName(9, 0)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 1)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 2)).toDouble()};

    // 显示初始条件和约束
    // Singleton<Logger>::getInstance()->logMessage("Initial_lower: " + QString::number(Initial_lower[0]) + ", " + QString::number(Initial_lower[1]) + ", " + QString::number(Initial_lower[2]), Logger::Info);
    // Singleton<Logger>::getInstance()->logMessage("Initial_upper: " + QString::number(Initial_upper[0]) + ", " + QString::number(Initial_upper[1]) + ", " + QString::number(Initial_upper[2]), Logger::Info);
    // Singleton<Logger>::getInstance()->logMessage("Initial_value: " + QString::number(Initial_value[0]) + ", " + QString::number(Initial_value[1]) + ", " + QString::number(Initial_value[2]), Logger::Info);

    // params = [532e-9, 12e9, 1, 100e6, 20e-3, 0.08, 0.00, 2.4e-4, 0.00, 1.3333, 10, 150, 0.04, 0.05, 1, 10, 2, 0.13, 0.4];
    // double params[19] = {532e-9, 12e9, 1, 100e6, 20e-3, 0.08, 0.00, 2.4e-4, 0.00, 1.33, 10, 150, 0.04, 0.05, 1, 10, 2, 0.13, 0.4};

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
    double tolerance = constantStorage->getConstant(constantMap->getConstantName(9, 12)).toDouble();

    double params[19] = {wave_length, freq_range, intensity, laser_width, laser_energy, alpha, beta_p, beta_m, beta_p, n, z, H, energy_ratio, r, M, N_dark, beta, xi, xi_f};

    double fitted_value[3];
    double resnorm;
    double exitflag;
    struct0_T output;
    struct1_T lambda;

    RetrievalAlgorithm(number, PMT_energy, Fizeau_spectrum, Initial_lower, Initial_upper, Initial_value, params, fitted_value, &resnorm, residual, &exitflag, &output, &lambda, jacobia, tolerance);

    // Singleton<Logger>::getInstance()->logMessage("PMT接收数据处理完成！", Logger::Info);

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

    // 存储到内存中
    constantStorage->setConstant(constantMap->getConstantName(6, 7), QVariant::fromValue(res_B_shift));
    constantStorage->setConstant(constantMap->getConstantName(6, 8), QVariant::fromValue(res_B_width));
    constantStorage->setConstant(constantMap->getConstantName(6, 9), QVariant::fromValue(res_R_width));
    constantStorage->setConstant(constantMap->getConstantName(6, 10), QVariant::fromValue(REF_Tem));
    constantStorage->setConstant(constantMap->getConstantName(6, 11), QVariant::fromValue(REF_Sal));

    delete xData;
    delete yData;
    // delete PMT_energy_vector;
}

// QMutex PMTReceptionDataGenerator::mutex;

QVector<double> *PMTReceptionDataGenerator::retrievalBySpecializePMT(QVector<double> *pmt_data, double z)
{
    // mutex.lock();
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);

    coder::array<double, 2U> PMT_energy;
    coder::array<double, 2U> Fizeau_spectrum;
    coder::array<double, 2U> residual;
    coder::array<double, 2U> jacobia;

    double number = constantStorage->getConstant(constantMap->getConstantName(3, 0)).toDouble();

    // QVector<double> *PMT_energy_vector = constantStorage->getConstant(constantMap->getConstantName(5, 14)).value<QVector<double> *>();

    // if (PMT_energy_vector == nullptr || PMT_energy_vector->isEmpty())
    // {
    //     Singleton<Logger>::getInstance()->logMessage("PMT_Energy为空！请先生成PMT接收数据！", Logger::Warning);
    //     return;
    // }

    // QSharedPointer<QCPGraphDataContainer> dataContainer;
    // //  constantStorage->getConstant(constantMap->getConstantName(5, 11))
    // dataContainer = constantStorage->getConstant(constantMap->getConstantName(5, 12)).value<QSharedPointer<QCPGraphDataContainer>>();

    QVector<double> *yData = FizeauIFGenerator::Fizeau_Polyarea;

    // if (dataContainer.isNull())
    // {
    //     Singleton<Logger>::getInstance()->logMessage("请先生成Fizeau仪器函数！", Logger::Warning);
    //     return;
    // }

    // QVector<double> *xData = new QVector<double>();
    // QVector<double> *yData = new QVector<double>();
    // constantStorage->convertQSharedPointerToQVector(dataContainer, xData, yData);

    // // 获取相对频率数据
    // QVector<double> *RF = FrequenceDataGenerator::generateRelativeFrequenceData();

    // // 先对Fizeau_spectrum进行面积归一化
    // double area = MyMath::polyarea(*RF, *yData);
    // for (int i = 0; i < yData->size(); ++i)
    // {
    //     (*yData)[i] = (*yData)[i] / area;
    // }
    // delete RF;

    MyMath::convertQVectorToArray(pmt_data, PMT_energy);
    MyMath::convertQVectorToArray(yData, Fizeau_spectrum);

    // double Initial_lower[3] = {7.0e9, 0.2e9, 0.1e9};
    // double Initial_upper[3] = {8.3e9, 1e9, 0.3e9};
    // double Initial_value[3] = {7.6732e9, 0.617e9, 0.15e9};

    // double Initial_lower[3] = {7.2e9, 0.2e9, 0.1e9}; // 初始值越小，拟合越精确，耗时越短
    // double Initial_upper[3] = {7.8e9, 0.8e9, 0.3e9};
    // double Initial_value[3] = {7.6732e9, 0.617e9, 0.15e9};
    double Initial_lower[3] = {constantStorage->getConstant(constantMap->getConstantName(9, 6)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 7)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 8)).toDouble()};

    double Initial_upper[3] = {constantStorage->getConstant(constantMap->getConstantName(9, 3)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 4)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 5)).toDouble()};

    double Initial_value[3] = {constantStorage->getConstant(constantMap->getConstantName(9, 0)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 1)).toDouble(),
                               constantStorage->getConstant(constantMap->getConstantName(9, 2)).toDouble()};

    // 显示初始条件和约束
    // Singleton<Logger>::getInstance()->logMessage("Initial_lower: " + QString::number(Initial_lower[0]) + ", " + QString::number(Initial_lower[1]) + ", " + QString::number(Initial_lower[2]), Logger::Info);
    // Singleton<Logger>::getInstance()->logMessage("Initial_upper: " + QString::number(Initial_upper[0]) + ", " + QString::number(Initial_upper[1]) + ", " + QString::number(Initial_upper[2]), Logger::Info);
    // Singleton<Logger>::getInstance()->logMessage("Initial_value: " + QString::number(Initial_value[0]) + ", " + QString::number(Initial_value[1]) + ", " + QString::number(Initial_value[2]), Logger::Info);

    // params = [532e-9, 12e9, 1, 100e6, 20e-3, 0.08, 0.00, 2.4e-4, 0.00, 1.3333, 10, 150, 0.04, 0.05, 1, 10, 2, 0.13, 0.4];
    // double params[19] = {532e-9, 12e9, 1, 100e6, 20e-3, 0.08, 0.00, 2.4e-4, 0.00, 1.33, 10, 150, 0.04, 0.05, 1, 10, 2, 0.13, 0.4};

    double wave_length = constantStorage->getConstant(constantMap->getConstantName(0, 1)).toDouble();
    double freq_range = constantStorage->getConstant(constantMap->getConstantName(0, 3)).toDouble();
    double intensity = constantStorage->getConstant(constantMap->getConstantName(0, 2)).toDouble();
    double laser_width = constantStorage->getConstant(constantMap->getConstantName(0, 0)).toDouble();
    double laser_energy = constantStorage->getConstant(constantMap->getConstantName(0, 5)).toDouble();
    double alpha = constantStorage->getConstant(constantMap->getConstantName(7, 1)).toDouble();
    double beta_p = constantStorage->getConstant(constantMap->getConstantName(1, 2)).toDouble(); // 有一个参数没用到
    double beta_m = constantStorage->getConstant(constantMap->getConstantName(1, 3)).toDouble();
    double n = constantStorage->getConstant(constantMap->getConstantName(1, 4)).toDouble();
    // double z = constantStorage->getConstant(constantMap->getConstantName(7, 6)).toDouble();
    double H = constantStorage->getConstant(constantMap->getConstantName(7, 7)).toDouble();
    double r = constantStorage->getConstant(constantMap->getConstantName(7, 4)).toDouble();
    double M = constantStorage->getConstant(constantMap->getConstantName(7, 0)).toDouble();
    double N_dark = constantStorage->getConstant(constantMap->getConstantName(7, 5)).toDouble();
    double beta = constantStorage->getConstant(constantMap->getConstantName(7, 8)).toDouble();
    double xi = constantStorage->getConstant(constantMap->getConstantName(7, 2)).toDouble();
    double xi_f = constantStorage->getConstant(constantMap->getConstantName(7, 3)).toDouble();
    double energy_ratio = constantStorage->getConstant(constantMap->getConstantName(1, 5)).toDouble();
    double tolerance = constantStorage->getConstant(constantMap->getConstantName(9, 12)).toDouble();

    double params[19] = {wave_length, freq_range, intensity, laser_width, laser_energy, alpha, beta_p, beta_m, beta_p, n, z, H, energy_ratio, r, M, N_dark, beta, xi, xi_f};

    double fitted_value[3];
    double resnorm;
    double exitflag;
    struct0_T output;
    struct1_T lambda;

    RetrievalAlgorithm(number, PMT_energy, Fizeau_spectrum, Initial_lower, Initial_upper, Initial_value, params, fitted_value, &resnorm, residual, &exitflag, &output, &lambda, jacobia, tolerance);

    // Singleton<Logger>::getInstance()->logMessage("PMT接收数据处理完成！", Logger::Info);

    //% 处理结果：
    // res.B_shift = fitted_value(1);
    // res.B_width = fitted_value(2);
    // res.R_width = fitted_value(3);
    // res.N_photo = fitted_value(4);

    double res_B_shift = fitted_value[0];
    double res_B_width = fitted_value[1];
    double res_R_width = fitted_value[2];
    // double res_N_photo = fitted_value[3];

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

    // qDebug() << "Result: " << REF_Tem << ", " << REF_Sal << Qt::endl;

    QVector<double> *result = new QVector<double>();
    result->append(res_B_shift);
    result->append(res_B_width);
    result->append(res_R_width);
    result->append(REF_Tem);
    result->append(REF_Sal);
    // delete PMT_energy_vector;
    delete pmt_data;
    // mutex.unlock();

    return result;
}
