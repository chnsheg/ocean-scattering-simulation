#include "underwaterspectrumdatagenerator.h"
#include "model/constantstorage.h"
#include "model/constantmap.h"
#include "utils/logger.h"
#include "utils/readfiledata.h"
#include "utils/mymath.h"
#include "matlab/AddNoiseNondB.h"
#include "matlab/drawSNRDepthByM.h"
#include "matlab/drawSNRDepthByAlpha.h"
#include "matlab/drawNsMByDepth.h"

UnderWaterSpectrumDataGenerator::UnderWaterSpectrumDataGenerator() {}

QVector<double> *UnderWaterSpectrumDataGenerator::caculateScatteredPhotonsByMatlabCode()
{
    // void CaculateScatteredPhotons(double energe, double Alpha_water,
    //                           double Beta_particulate, double Beta_m,
    //                           double lambda, double n, double z, double H,
    //                           double energy_ratio, double *N_Brillouin,
    //                           double *N_Mie, double *N_Rayleigh)

    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);
    double energe = constantStorage->getConstant(constantMap->getConstantName(0, 5)).toDouble();
    double Alpha_water = constantStorage->getConstant(constantMap->getConstantName(7, 1)).toDouble();
    double Beta_particulate = constantStorage->getConstant(constantMap->getConstantName(1, 2)).toDouble();
    double Beta_m = constantStorage->getConstant(constantMap->getConstantName(1, 3)).toDouble();
    double lambda = constantStorage->getConstant(constantMap->getConstantName(0, 1)).toDouble();
    double n = constantStorage->getConstant(constantMap->getConstantName(1, 4)).toDouble();
    double z = constantStorage->getConstant(constantMap->getConstantName(7, 6)).toDouble();
    double H = constantStorage->getConstant(constantMap->getConstantName(7, 7)).toDouble();
    double energy_ratio = constantStorage->getConstant(constantMap->getConstantName(1, 5)).toDouble();
    double N_Brillouin;
    double N_Mie;
    double N_Rayleigh;
    double SNR;
    double laser_width = constantStorage->getConstant(constantMap->getConstantName(0, 0)).toDouble();
    double tau = 1 / laser_width;
    double r = constantStorage->getConstant(constantMap->getConstantName(7, 4)).toDouble();
    double M = constantStorage->getConstant(constantMap->getConstantName(7, 0)).toDouble();
    double N_dark = constantStorage->getConstant(constantMap->getConstantName(7, 5)).toDouble();
    double beta = constantStorage->getConstant(constantMap->getConstantName(7, 8)).toDouble();
    double Xi = constantStorage->getConstant(constantMap->getConstantName(7, 2)).toDouble();
    double Xi_F = constantStorage->getConstant(constantMap->getConstantName(7, 3)).toDouble();

    // void CaculateScatteredPhotons(double energe, double Alpha_water, double Beta_p,
    //                           double Beta_m, double lambda, double n, double z,
    //                           double H, double tau, double r, double M,
    //                           double energy_ratio, double N_dark, double beta,
    //                           double Xi, double Xi_F, double *N_Brillouin,
    //                           double *N_Mie, double *N_Rayleigh, double *SNR)

    // CaculateScatteredPhotons(energe, Alpha_water, Beta_particulate, Beta_m, lambda, n, z, H, energy_ratio, &N_Brillouin, &N_Mie, &N_Rayleigh, &SNR);
    CaculateScatteredPhotons(energe, Alpha_water, Beta_particulate, Beta_m, lambda, n, z, H, tau, r, M, energy_ratio, N_dark, beta, Xi, Xi_F, &N_Brillouin, &N_Mie, &N_Rayleigh, &SNR);
    QVector<double> *result = new QVector<double>({N_Brillouin, N_Rayleigh, N_Mie, SNR});

    Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(6, 2), N_Brillouin);
    Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(6, 3), N_Mie);
    Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(6, 4), N_Rayleigh);
    Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(6, 5), SNR);
    Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(6, 6), N_Brillouin + N_Rayleigh + N_Mie);

    return result;
}

QVector<QVector<double> *> *UnderWaterSpectrumDataGenerator::generateUnderWaterSpectrumData()
{
    QVector<double> *result = caculateScatteredPhotonsByMatlabCode();
    for (int i = 0; i < result->size(); ++i)
    {
        Singleton<Logger>::getInstance()->logMessage("result: " + QString::number(result->at(i)), Logger::Log);
    }
    QVector<QVector<double> *> *xDataVectorContainer;
    QVector<QVector<double> *> *yDataVectorContainer;
    xDataVectorContainer = new QVector<QVector<double> *>();
    yDataVectorContainer = new QVector<QVector<double> *>();
    for (int i = 0; i < 4; ++i)
    {
        // 预先分配4次内存
        xDataVectorContainer->append(new QVector<double>());
        yDataVectorContainer->append(new QVector<double>());
    }

    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);
    QSharedPointer<QCPGraphDataContainer> dataContainer;

    // 从存储中获取散射光谱
    for (int i = 0; i < 3; ++i)
    {
        dataContainer = constantStorage->getConstant(constantMap->getConstantName(5, i + 4)).value<QSharedPointer<QCPGraphDataContainer>>();
        if (dataContainer.isNull())
        {
            Singleton<Logger>::getInstance()->logMessage("散射光谱为空！请先生成散射光谱！", Logger::Warning);
            return nullptr;
        }
        constantStorage->convertQSharedPointerToQVector(dataContainer, xDataVectorContainer->at(i), yDataVectorContainer->at(i));
    }

    QVector<double> *L_b = yDataVectorContainer->at(0);
    QVector<double> *L_r = yDataVectorContainer->at(1);
    QVector<double> *L_m = yDataVectorContainer->at(2);
    // ReadFileData::saveDataToCSVFile(xDataVectorContainer->at(0), L_r, "L_rc.csv");
    // ReadFileData::saveDataToCSVFile(xDataVectorContainer->at(0), L_m, "L_mc.csv");
    // ReadFileData::saveDataToCSVFile(xDataVectorContainer->at(0), L_b, "L_bc.csv");

    // SpectraR = L_rc * Num_Rayleigh; %Rayleigh Spectra   光子数*分布函数 = 光谱 I*S(v)
    // SpectraM = L_mc * Num_Mie; %Mie Spectrum?           乘以光子数后，纵轴的单位是光子数，因此可以比较不同的散射谱的强度
    // SpectraB = L_bc * Num_Brillouin; %Brillouin Spectrum

    int size = L_b->size();
    QVector<double> *L_rc = new QVector<double>(size);
    QVector<double> *L_mc = new QVector<double>(size);
    QVector<double> *L_bc = new QVector<double>(size);

    for (int i = 0; i < L_b->size(); ++i)
    {
        L_bc->replace(i, L_b->at(i) * result->at(0));
        L_rc->replace(i, L_r->at(i) * result->at(1));
        L_mc->replace(i, L_m->at(i) * result->at(2));
    }

    // Spectrum1 = SpectraR + SpectraM + SpectraB
    QVector<double> *Spectrum1 = new QVector<double>(size);
    for (int i = 0; i < size; ++i)
    {
        // Spectrum1->append(L_rc->at(i) + L_mc->at(i) + L_bc->at(i));
        Spectrum1->replace(i, L_rc->at(i) + L_mc->at(i) + L_bc->at(i));
    }

    // 加噪
    // void AddNoiseNondB(const coder::array<double, 2U> &Iv, double SNR,
    //                coder::array<double, 2U> &Iv_Noised, double *SNR_cal)
    double SNR = result->at(3);
    coder::array<double, 2U> Iv;
    coder::array<double, 2U> Iv_Noised;
    QVector<double> *Spectrum_Noised;
    MyMath::convertQVectorToArray(Spectrum1, Iv);
    double poissrnd_lambda = constantStorage->getConstant(constantMap->getConstantName(3, 3)).toDouble();
    qDebug() << "poissrnd_lambda: " << poissrnd_lambda;

    AddNoiseNondB(Iv, SNR, Iv_Noised, &SNR, poissrnd_lambda);
    Spectrum_Noised = MyMath::convertArrayToQVector(Iv_Noised);

    // ReadFileData::saveDataToCSVFile(xDataVectorContainer->at(0), L_rc, "L_rc.csv");
    // ReadFileData::saveDataToCSVFile(xDataVectorContainer->at(0), L_mc, "L_mc.csv");
    // ReadFileData::saveDataToCSVFile(xDataVectorContainer->at(0), L_bc, "L_bc.csv");
    // ReadFileData::saveDataToCSVFile(xDataVectorContainer->at(0), Spectrum1, "Spectrum1.csv");

    QVector<QVector<double> *> *resultContainer = new QVector<QVector<double> *>({L_rc, L_mc, L_bc, Spectrum_Noised});
    // QVector<QVector<double> *> *resultContainer = new QVector<QVector<double> *>({L_b, L_r, L_m, L_mc});
    delete Spectrum1;
    delete result;
    delete L_b;
    delete L_r;
    delete L_m;
    delete xDataVectorContainer;
    delete yDataVectorContainer;

    return resultContainer;
}

QVector<QVector<double> *> *UnderWaterSpectrumDataGenerator::generateSNRDepthByMData()
{
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);
    double energe = constantStorage->getConstant(constantMap->getConstantName(0, 5)).toDouble();
    double Alpha_water = constantStorage->getConstant(constantMap->getConstantName(7, 1)).toDouble();
    double Beta_particulate = constantStorage->getConstant(constantMap->getConstantName(1, 2)).toDouble();
    double Beta_m = constantStorage->getConstant(constantMap->getConstantName(1, 3)).toDouble();
    double lambda = constantStorage->getConstant(constantMap->getConstantName(0, 1)).toDouble();
    double n = constantStorage->getConstant(constantMap->getConstantName(1, 4)).toDouble();
    // double z = constantStorage->getConstant(constantMap->getConstantName(7, 6)).toDouble();
    double H = constantStorage->getConstant(constantMap->getConstantName(7, 7)).toDouble();
    double energy_ratio = constantStorage->getConstant(constantMap->getConstantName(1, 5)).toDouble();
    // double N_Brillouin;
    // double N_Mie;
    // double N_Rayleigh;
    // double SNR;
    double laser_width = constantStorage->getConstant(constantMap->getConstantName(0, 0)).toDouble();
    double tau = 1 / laser_width;
    double r = constantStorage->getConstant(constantMap->getConstantName(7, 4)).toDouble();
    // double M = constantStorage->getConstant(constantMap->getConstantName(7, 0)).toDouble();
    double N_dark = constantStorage->getConstant(constantMap->getConstantName(7, 5)).toDouble();
    double beta = constantStorage->getConstant(constantMap->getConstantName(7, 8)).toDouble();
    double Xi = constantStorage->getConstant(constantMap->getConstantName(7, 2)).toDouble();
    double Xi_F = constantStorage->getConstant(constantMap->getConstantName(7, 3)).toDouble();

    // void drawSNRDepthByM(double energe, double Alpha_water, double Beta_p,
    //                  double Beta_m, double lambda, double n,
    //                  const coder::array<double, 2U> &z, double H, double tau,
    //                  double r, const coder::array<double, 2U> &M,
    //                  double energy_ratio, double N_dark, double beta, double Xi,
    //                  double Xi_F, coder::array<double, 2U> &N_Brillouin,
    //                  coder::array<double, 2U> &N_Mie,
    //                  coder::array<double, 2U> &N_Rayleigh,
    //                  coder::array<double, 2U> &SNR_db)

    coder::array<double, 2U> z;
    coder::array<double, 2U> M;
    coder::array<double, 2U> N_Brillouin;
    coder::array<double, 2U> N_Mie;
    coder::array<double, 2U> N_Rayleigh;
    coder::array<double, 2U> SNR_db;
    QVector<double> *zVector = new QVector<double>();
    QVector<double> *MVector = new QVector<double>();
    QVector<double> *N_BrillouinVector;
    QVector<double> *N_MieVector;
    QVector<double> *N_RayleighVector;
    QVector<QVector<double> *> *SNR_dbVector;

    for (int i = 0; i < 150; ++i)
    {
        zVector->append(i);
    }

    MVector->append(1);
    MVector->append(100);
    MVector->append(1000);
    MVector->append(10000);

    MyMath::convertQVectorToArray(zVector, z);
    MyMath::convertQVectorToArray(MVector, M);

    drawSNRDepthByM(energe, Alpha_water, Beta_particulate, Beta_m, lambda, n, z, H, tau, r, M, energy_ratio, N_dark, beta, Xi, Xi_F, N_Brillouin, N_Mie, N_Rayleigh, SNR_db);

    N_BrillouinVector = MyMath::convertArrayToQVector(N_Brillouin);
    N_MieVector = MyMath::convertArrayToQVector(N_Mie);
    N_RayleighVector = MyMath::convertArrayToQVector(N_Rayleigh);
    SNR_dbVector = MyMath::convertMultiArrayToQVector(SNR_db);

    QVector<QVector<double> *> *resultContainer = new QVector<QVector<double> *>();
    resultContainer->append(zVector);
    for (int i = 0; i < MVector->size(); ++i)
    {
        resultContainer->append(SNR_dbVector->at(i));
    }
    resultContainer->append(MVector);

    delete N_BrillouinVector;
    delete N_MieVector;
    delete N_RayleighVector;

    return resultContainer;
}

QVector<QVector<double> *> *UnderWaterSpectrumDataGenerator::generateSNRDepthByAlphaData()
{
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);
    double energe = constantStorage->getConstant(constantMap->getConstantName(0, 5)).toDouble();
    // double Alpha_water = constantStorage->getConstant(constantMap->getConstantName(7, 1)).toDouble();
    double Beta_particulate = constantStorage->getConstant(constantMap->getConstantName(1, 2)).toDouble();
    double Beta_m = constantStorage->getConstant(constantMap->getConstantName(1, 3)).toDouble();
    double lambda = constantStorage->getConstant(constantMap->getConstantName(0, 1)).toDouble();
    double n = constantStorage->getConstant(constantMap->getConstantName(1, 4)).toDouble();
    // double z = constantStorage->getConstant(constantMap->getConstantName(7, 6)).toDouble();
    double H = constantStorage->getConstant(constantMap->getConstantName(7, 7)).toDouble();
    double energy_ratio = constantStorage->getConstant(constantMap->getConstantName(1, 5)).toDouble();
    // double N_Brillouin;
    // double N_Mie;
    // double N_Rayleigh;
    // double SNR;
    double laser_width = constantStorage->getConstant(constantMap->getConstantName(0, 0)).toDouble();
    double tau = 1 / laser_width;
    double r = constantStorage->getConstant(constantMap->getConstantName(7, 4)).toDouble();
    double M = constantStorage->getConstant(constantMap->getConstantName(7, 0)).toDouble();
    double N_dark = constantStorage->getConstant(constantMap->getConstantName(7, 5)).toDouble();
    double beta = constantStorage->getConstant(constantMap->getConstantName(7, 8)).toDouble();
    double Xi = constantStorage->getConstant(constantMap->getConstantName(7, 2)).toDouble();
    double Xi_F = constantStorage->getConstant(constantMap->getConstantName(7, 3)).toDouble();

    coder::array<double, 2U> z;
    coder::array<double, 2U> Alpha_water;
    coder::array<double, 2U> N_Brillouin;
    coder::array<double, 2U> N_Mie;
    coder::array<double, 2U> N_Rayleigh;
    coder::array<double, 2U> SNR_db;
    QVector<double> *zVector = new QVector<double>();
    QVector<double> *AlphaVector = new QVector<double>();
    QVector<QVector<double> *> *SNR_dbVector;

    for (int i = 0; i < 40; ++i)
    {
        zVector->append(i);
    }

    AlphaVector->append(0.05);
    AlphaVector->append(0.08);
    AlphaVector->append(0.1);
    AlphaVector->append(0.2);

    MyMath::convertQVectorToArray(zVector, z);
    MyMath::convertQVectorToArray(AlphaVector, Alpha_water);

    drawSNRDepthByAlpha(energe, Alpha_water, Beta_particulate, Beta_m, lambda, n, z, H, tau, r, M, energy_ratio, N_dark, beta, Xi, Xi_F, SNR_db);

    SNR_dbVector = MyMath::convertMultiArrayToQVector(SNR_db);

    QVector<QVector<double> *> *resultContainer = new QVector<QVector<double> *>();

    resultContainer->append(zVector);
    for (int i = 0; i < AlphaVector->size(); ++i)
    {
        resultContainer->append(SNR_dbVector->at(i));
    }
    resultContainer->append(AlphaVector);

    return resultContainer;
}

QVector<QVector<double> *> *UnderWaterSpectrumDataGenerator::generateNsMByDepthData()
{
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);
    double energe = constantStorage->getConstant(constantMap->getConstantName(0, 5)).toDouble();
    double Alpha_water = constantStorage->getConstant(constantMap->getConstantName(7, 1)).toDouble();
    double Beta_particulate = constantStorage->getConstant(constantMap->getConstantName(1, 2)).toDouble();
    double Beta_m = constantStorage->getConstant(constantMap->getConstantName(1, 3)).toDouble();
    double lambda = constantStorage->getConstant(constantMap->getConstantName(0, 1)).toDouble();
    double n = constantStorage->getConstant(constantMap->getConstantName(1, 4)).toDouble();
    // double z = constantStorage->getConstant(constantMap->getConstantName(7, 6)).toDouble();
    double H = constantStorage->getConstant(constantMap->getConstantName(7, 7)).toDouble();
    double energy_ratio = constantStorage->getConstant(constantMap->getConstantName(1, 5)).toDouble();
    // double N_Brillouin;
    // double N_Mie;
    // double N_Rayleigh;
    // double SNR;
    double laser_width = constantStorage->getConstant(constantMap->getConstantName(0, 0)).toDouble();
    double tau = 1 / laser_width;
    double r = constantStorage->getConstant(constantMap->getConstantName(7, 4)).toDouble();
    // double M = constantStorage->getConstant(constantMap->getConstantName(7, 0)).toDouble();
    double N_dark = constantStorage->getConstant(constantMap->getConstantName(7, 5)).toDouble();
    double beta = constantStorage->getConstant(constantMap->getConstantName(7, 8)).toDouble();
    double Xi = constantStorage->getConstant(constantMap->getConstantName(7, 2)).toDouble();
    double Xi_F = constantStorage->getConstant(constantMap->getConstantName(7, 3)).toDouble();

    // void drawSNRDepthByM(double energe, double Alpha_water, double Beta_p,
    //                  double Beta_m, double lambda, double n,
    //                  const coder::array<double, 2U> &z, double H, double tau,
    //                  double r, const coder::array<double, 2U> &M,
    //                  double energy_ratio, double N_dark, double beta, double Xi,
    //                  double Xi_F, coder::array<double, 2U> &N_Brillouin,
    //                  coder::array<double, 2U> &N_Mie,
    //                  coder::array<double, 2U> &N_Rayleigh,
    //                  coder::array<double, 2U> &SNR_db)

    coder::array<double, 2U> z;
    coder::array<double, 2U> M;
    coder::array<double, 2U> N_Brillouin;
    coder::array<double, 2U> N_Mie;
    coder::array<double, 2U> N_Rayleigh;
    coder::array<double, 2U> Ns;
    QVector<double> *zVector = new QVector<double>();
    QVector<double> *MVector = new QVector<double>();
    QVector<double> *N_BrillouinVector;
    QVector<double> *N_MieVector;
    QVector<double> *N_RayleighVector;
    QVector<QVector<double> *> *NsVector;

    for (int i = 0; i < 40; ++i)
    {
        zVector->append(i);
    }

    MVector->append(10);
    MVector->append(100);
    MVector->append(1000);
    MVector->append(10000);

    MyMath::convertQVectorToArray(zVector, z);
    MyMath::convertQVectorToArray(MVector, M);

    drawNsMByDepth(energe, Alpha_water, Beta_particulate, Beta_m, lambda, n, z, H, tau, r, M, energy_ratio, N_dark, beta, Xi, Xi_F, N_Brillouin, N_Mie, N_Rayleigh, Ns);

    N_BrillouinVector = MyMath::convertArrayToQVector(N_Brillouin);
    N_MieVector = MyMath::convertArrayToQVector(N_Mie);
    N_RayleighVector = MyMath::convertArrayToQVector(N_Rayleigh);
    NsVector = MyMath::convertMultiArrayToQVector(Ns);

    QVector<QVector<double> *> *resultContainer = new QVector<QVector<double> *>();
    resultContainer->append(zVector);
    for (int i = 0; i < MVector->size(); ++i)
    {
        resultContainer->append(NsVector->at(i));
    }
    resultContainer->append(MVector);

    delete N_BrillouinVector;
    delete N_MieVector;
    delete N_RayleighVector;

    return resultContainer;
}