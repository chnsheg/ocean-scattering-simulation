#include "model/laserdatagenerator.h"
#include <QDebug>
#include "utils/readfiledata.h"
#include "QMath.h"

double polyarea(const QVector<double> &x, const QVector<double> &y)
{
    double area = 0.0;
    int j = x.size() - 1;

    for (int i = 0; i < x.size(); i++)
    {
        area += (x[j] + x[i]) * (y[j] - y[i]);
        j = i;
    }

    return std::abs(area / 2.0);
}

QVector<double> *LaserDataGenerator::generateLaserData()
{
    // 读取存储的数据
    double width = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 0)).toDouble();
    double lambda_0 = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 1)).toDouble();
    // QString s = Singleton<ConstantMap>::getInstance()->getConstantName(0, 0);
    double c = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 4)).toDouble();
    double intensity = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 2)).toDouble();
    double frequencyRange = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(0, 3)).toDouble();

    // 计算得到的变量
    double delta_l = width / 2.0;
    double step = 1e6;
    double miu_0 = c / lambda_0;
    double lowerLimit = miu_0 - frequencyRange;
    double upperLimit = miu_0 + frequencyRange;
    int size = (upperLimit - lowerLimit) / step + 1;
    QVector<double> miu(size), fx(size);
    fx.fill(0);
    int max_index = 0;
    for (int i = 0; i < size; i++)
    {
        miu[i] = lowerLimit + i * step;
        // fx(i) = Intensity * exp(- ((x - miu_0) ^ 2) / (2 * delta_L ^ 2)); % 高斯分布
        fx[i] = intensity * qExp(-qPow((miu[i] - miu_0), 2) / (2 * qPow(delta_l, 2)));
        // 记录fx最大值
        if (fx[i] > fx[max_index])
        {
            max_index = i;
        }
    }

    // D = polyarea(miu, fx); % 求面积，面积归一化
    double D = polyarea(miu, fx);
    double max = fx[max_index] / D;

    QVector<double> *data = new QVector<double>(size);
    for (int i = 0; i < size; i++)
    {
        (*data)[i] = fx[i] / D / max;
    }

    QVector<double> *data1 = new QVector<double>;
    ReadFileData::readCSV(data1, "laser_data.csv");

    // 对比data和data1两个数据,如果每个点都相等，则打印相等，否则打印不相等
    // bool flag = true;
    // for (int i = 0; i < data->size(); i++)
    // {
    //     if (qAbs((*data)[i] - (*data1)[i]) > 1e-4)
    //     {
    //         flag = false;
    //         break;
    //     }
    // }
    // if (flag)
    // {
    //     qDebug() << "data == data1";
    // }
    // else
    // {
    //     qDebug() << "data != data1";
    // }

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
    double width_lase = miu[col_LS.back()] - miu[col_LS.front()];
    qDebug() << "width_lase: " << width_lase;

    return data1;
}
