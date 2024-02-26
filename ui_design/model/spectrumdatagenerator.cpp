#include "model/spectrumdatagenerator.h"
#include <QDebug>
#include <qmath.h>
#include "utils/mymath.h"
#include "model/frequencedatagenerator.h"

double SpectrumDataGenerator::BrillouinLine()
{
    double t = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(1, 0)).toDouble();
    double S = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(1, 1)).toDouble();
    double a = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 1)).toDouble() * 1e9; // 激光波长

    // 计算折射率n
    double n0 = 1.31405;
    double n1 = 1.779e-4;
    double n2 = -1.05e-6;
    double n3 = 1.6e-8;
    double n4 = -2.02e-6;
    double n5 = 15.868;
    double n6 = 0.01155;
    double n7 = -0.00423;
    double n8 = -4382;
    double n9 = 1.1455e6;
    double n = n0 + S * (n1 + n2 * t + n3 * t * t) + n4 * t * t + (n5 + n6 * S + n7 * t) / a + n8 / (a * a) + n9 / (a * a * a);

    // 计算剪切粘滞系数gs和体粘滞系数gb
    double q00 = 1.79e-2;
    double q01 = -6.1299e-4;
    double q02 = 1.4467e-5;
    double q03 = -1.6826e-7;
    double r0 = 2.4727e-5;
    double r1 = 4.8429e-7;
    double r2 = -4.7172e-8;
    double r3 = 7.5986e-10;
    double gs = 0.1 * (q00 + q01 * t + q02 * t * t + q03 * t * t * t + S * (r0 + r1 * t + r2 * t * t + r3 * t * t * t));
    double ratio = 4.8840 - 0.2488 * t + 7.33e-3 * t * t - 6.125e-5 * t * t * t;
    double gb = gs * ratio;

    // 计算海水密度rho
    double a0 = 999.842594;
    double a1 = 6.793952e-2;
    double a2 = -9.095290e-3;
    double a3 = 1.001685e-4;
    double a4 = -1.120083e-6;
    double a5 = 6.536332e-9;
    double b0 = 8.24493e-1;
    double b1 = -4.0899e-3;
    double b2 = 7.6438e-5;
    double b3 = -8.2467e-7;
    double b4 = 5.3875e-9;
    double c0 = -5.72466e-3;
    double c1 = 1.0227e-4;
    double c2 = -1.6546e-6;
    double d0 = 4.8314e-4;
    double rho = a0 + a1 * t + a2 * t * t + a3 * t * t * t + a4 * t * t * t * t + a5 * t * t * t * t * t +
                 S * (b0 + b1 * t + b2 * t * t + b3 * t * t * t + b4 * t * t * t * t) +
                 S * sqrt(S) * (c0 + c1 * t + c2 * t * t) + d0 * S * S;

    // 计算线宽linew
    double linew = 1.9850e18 * (1.0 / (4.0 * MY_PI * rho)) * qPow((4.0 * MY_PI * n / a), 2) * (4.0 / 3.0 * gs + gb);

    Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(1, 2), linew);
    qDebug() << "rho: " << rho;
    qDebug() << "n: " << n;
    qDebug() << "gs: " << gs;
    qDebug() << "gb: " << gb;
    qDebug() << "linew: " << linew;
    return linew;
}

double SpectrumDataGenerator::BrillouinShift()
{
    double t = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(1, 0)).toDouble();
    double S = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(1, 1)).toDouble();
    double a = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 1)).toDouble() * 1e9; // 激光波长
    // 计算声速
    double c0 = 1402.392, c1 = 5.01109398873, c2 = -0.0550946843172, c3 = 0.00022153596924,
           c4 = 1.32952290781, c5 = 0.000128955756844, c6 = -0.0127562783426, c7 = 0.000096840315641;
    double c = c0 + c1 * t + c2 * t * t + c3 * t * t * t + c4 * S + c5 * S * S + c6 * t * S + c7 * t * t * S;

    // 计算折射率n
    double n0 = 1.31405, n1 = 1.779 * 1e-4, n2 = -1.05 * 1e-6, n3 = 1.6 * 1e-8,
           n4 = -2.02 * 1e-6, n5 = 15.868, n6 = 0.01155, n7 = -0.00423, n8 = -4382, n9 = 1.1455 * 1e6;

    double n = n0 + S * (n1 + n2 * t + n3 * t * t) + n4 * t * t + (n5 + n6 * S + n7 * t) / a + n8 / a / a + n9 / a / a / a;

    // 计算 Brillouin 频移
    double v = 2 * n * c / a / 1e-9; // p25面式(2.27)

    Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(1, 3), v);

    qDebug() << "v: " << v;
    return v;
}

QVector<double> *SpectrumDataGenerator::generateBriScatteringData()
{
    double vr = Singleton<ConstantStorage>::getInstance(nullptr)
                    ->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 4))
                    .toDouble() /
                Singleton<ConstantStorage>::getInstance(nullptr)
                    ->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 1))
                    .toDouble();
    double v_b = BrillouinShift();
    double v_b1 = vr - v_b;
    double v_b2 = vr + v_b;

    double delta_b = BrillouinLine();

    QVector<double> *miu = FrequenceDataGenerator::generateFrequenceData();
    int size = miu->size();
    QVector<double> *data = new QVector<double>(size);

    // L_m = delta_m ./ (4 .* (miu - vr) .^ 2 + delta_m ^ 2); % 定义瑞利峰，线型为洛伦兹函数
    for (int i = 0; i < size; i++)
    {
        (*data)[i] = delta_b / (4 * qPow((*miu)[i] - vr, 2) + qPow(delta_b, 2));
    }

    double A = MyMath::polyarea(*miu, *data);
    for (int i = 0; i < size; i++)
    {
        (*data)[i] = (*data)[i] / A;
    }

    delete miu;
    return data;
}

QVector<double> *SpectrumDataGenerator::generateMieScatteringData()
{
    double delta_m = 0.04e9; // 米散射线宽(20~30MHz)，单位GHz,和激光线宽一样
    double vr = Singleton<ConstantStorage>::getInstance(nullptr)
                    ->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 4))
                    .toDouble() /
                Singleton<ConstantStorage>::getInstance(nullptr)
                    ->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 1))
                    .toDouble();

    QVector<double> *miu = FrequenceDataGenerator::generateFrequenceData();
    int size = miu->size();
    QVector<double> *data = new QVector<double>(size);
    for (int i = 0; i < size; i++)
    {
        (*data)[i] = delta_m / (4 * qPow((*miu)[i] - vr, 2) + qPow(delta_m, 2));
    }

    double A = MyMath::polyarea(*miu, *data);
    for (int i = 0; i < size; i++)
    {
        (*data)[i] = (*data)[i] / A;
    }

    delete miu;
    return data;
}

QVector<double> *SpectrumDataGenerator::generateRayScatteringData()
{
    double delta_r = 0.15e9; // 瑞利线宽(150MHz)，单位GHz %瑞利线宽比米散射宽
    double vr = Singleton<ConstantStorage>::getInstance(nullptr)
                    ->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 4))
                    .toDouble() /
                Singleton<ConstantStorage>::getInstance(nullptr)
                    ->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 1))
                    .toDouble();

    QVector<double> *miu = FrequenceDataGenerator::generateFrequenceData();
    int size = miu->size();
    QVector<double> *data = new QVector<double>(size);
    for (int i = 0; i < size; i++)
    {
        (*data)[i] = delta_r / (4 * qPow((*miu)[i] - vr, 2) + qPow(delta_r, 2));
    }

    double A = MyMath::polyarea(*miu, *data);
    for (int i = 0; i < size; i++)
    {
        (*data)[i] = (*data)[i] / A;
    }

    delete miu;

    return data;
}
