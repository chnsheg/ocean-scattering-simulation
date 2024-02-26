#ifndef MYMATH_H
#define MYMATH_H

#include <cmath>
#include <QVector>

#ifndef MY_PI
#define MY_PI 3.14159265358979323846
#endif

class MyMath
{
public:
    MyMath();

    static double polyarea(const QVector<double> &x, const QVector<double> &y);
};

#endif // MYMATH_H
