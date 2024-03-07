#include "underwaterspectrumdatagenerator.h"
#include "model/constantstorage.h"
#include "model/constantmap.h"
#include "utils/logger.h"

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
    double energe = constantStorage->getConstant(constantMap->getConstantName(3, 0)).toDouble();
    double Alpha_water = constantStorage->getConstant(constantMap->getConstantName(3, 1)).toDouble();
    double Beta_particulate = constantStorage->getConstant(constantMap->getConstantName(3, 2)).toDouble();
    double Beta_m = constantStorage->getConstant(constantMap->getConstantName(3, 3)).toDouble();
    double lambda = constantStorage->getConstant(constantMap->getConstantName(3, 4)).toDouble();
    double n = constantStorage->getConstant(constantMap->getConstantName(3, 5)).toDouble();
    double z = constantStorage->getConstant(constantMap->getConstantName(3, 6)).toDouble();
    double H = constantStorage->getConstant(constantMap->getConstantName(3, 7)).toDouble();
    double energy_ratio = constantStorage->getConstant(constantMap->getConstantName(3, 8)).toDouble();
    double N_Brillouin;
    double N_Mie;
    double N_Rayleigh;

    CaculateScatteredPhotons(energe, Alpha_water, Beta_particulate, Beta_m, lambda, n, z, H, energy_ratio, &N_Brillouin, &N_Mie, &N_Rayleigh);
    QVector<double> *result = new QVector<double>({N_Brillouin, N_Rayleigh, N_Mie});

    Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(6, 2), N_Brillouin);
    Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(6, 3), N_Mie);
    Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(6, 4), N_Rayleigh);

    return result;
}

QVector<QVector<double> *> *UnderWaterSpectrumDataGenerator::generateUnderWaterSpectrumData()
{
    QVector<double> *result = caculateScatteredPhotonsByMatlabCode();
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
        dataContainer = constantStorage->getConstant(constantMap->getConstantName(5, i + 8)).value<QSharedPointer<QCPGraphDataContainer>>();
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

    // SpectraR = L_rc * Num_Rayleigh; %Rayleigh Spectra   光子数*分布函数 = 光谱 I*S(v)
    // SpectraM = L_mc * Num_Mie; %Mie Spectrum?           乘以光子数后，纵轴的单位是光子数，因此可以比较不同的散射谱的强度
    // SpectraB = L_bc * Num_Brillouin; %Brillouin Spectrum

    int size = L_b->size();
    QVector<double> *L_rc = new QVector<double>(size);
    QVector<double> *L_mc = new QVector<double>(size);
    QVector<double> *L_bc = new QVector<double>(size);

    for (int i = 0; i < L_b->size(); ++i)
    {
        L_bc->append(L_b->at(i) * result->at(0));
        L_rc->append(L_r->at(i) * result->at(1));
        L_mc->append(L_m->at(i) * result->at(2));
    }

    // Spectrum1 = SpectraR + SpectraM + SpectraB
    QVector<double> *Spectrum1 = new QVector<double>(size);
    for (int i = 0; i < size; ++i)
    {
        Spectrum1->append(L_rc->at(i) + L_mc->at(i) + L_bc->at(i));
    }

    QVector<QVector<double> *> *resultContainer = new QVector<QVector<double> *>({L_rc, L_mc, L_bc, Spectrum1});

    delete result;
    delete L_b;
    delete L_r;
    delete L_m;
    delete xDataVectorContainer;
    delete yDataVectorContainer;

    return resultContainer;
}