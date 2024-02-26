#include "mymath.h"

MyMath::MyMath() {}

double MyMath::polyarea(const QVector<double> &x, const QVector<double> &y)
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