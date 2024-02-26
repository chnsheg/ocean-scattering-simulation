#include "model/laserdatagenerator.h"
#include <QDebug>
#include "QMath.h"
#include "model/frequencedatagenerator.h"
#include "utils/mymath.h"

QVector<double> *LaserDataGenerator::generateLaserData()
{
    // 读取存储的数据
    double width = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 0)).toDouble();
    double lambda_0 = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 1)).toDouble();
    double c = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 4)).toDouble();
    double intensity = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 2)).toDouble();

    // 计算得到的变量
    double delta_l = width / 2.0;
    double miu_0 = c / lambda_0;

    QVector<double> *miu = FrequenceDataGenerator::generateFrequenceData();
    int size = miu->size();
    QVector<double> fx(size);

    fx.fill(0);
    int max_index = 0;
    for (int i = 0; i < size; i++)
    {
        fx[i] = intensity * qExp(-qPow((*miu)[i] - miu_0, 2) / (2 * qPow(delta_l, 2)));
        // 记录fx最大值
        if (fx[i] > fx[max_index])
        {
            max_index = i;
        }
    }

    // D = polyarea(miu, fx); % 求面积，面积归一化
    double D = MyMath::polyarea(*miu, fx);
    double max = fx[max_index] / D;

    QVector<double> *data = new QVector<double>(size);
    for (int i = 0; i < size; i++)
    {
        (*data)[i] = fx[i] / D / max;
    }

    // 寻找 LS 在 (0.499, 0.519) 范围内的点
    std::vector<int> col_LS;
    for (int i = 0; i < size; ++i)
    {
        if ((*data)[i] > 0.499 && (*data)[i] < 0.519)
        {
            col_LS.push_back(i);
        }
    }

    // 计算宽度
    // double width_lase = (*miu)[col_LS.back()] - (*miu)[col_LS.front()];
    double width_lase = (*miu)[col_LS.back()] - (*miu)[col_LS.front()];
    qDebug() << "width_lase: " << width_lase;

    delete miu;
    miu = nullptr;

    return data;
}
