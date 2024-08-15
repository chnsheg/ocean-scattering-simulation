#ifndef MYMATH_H
#define MYMATH_H

#include <cmath>
#include <QVector>
#include "matlab/coder_array.h"
#include "fftw3.h"

#ifndef MY_PI
#define MY_PI 3.14159265358979323846
#endif
#include <QMutex>

class MyMath
{
public:
    MyMath();
    static QVector<double> *convertArrayToQVector(const coder::array<double, 2U> &array);
    static void convertQVectorToArray(const QVector<double> *vector, coder::array<double, 2U> &array);
    // 将多维的coder::array 转换为 QVector<QVector<double> *>*
    static QVector<QVector<double> *> *convertMultiArrayToQVector(const coder::array<double, 2U> &array);
    static double polyarea(const QVector<double> &x, const QVector<double> &y);
    static QVector<double> *convolution(QVector<double> *input, QVector<double> *kernel);
    static void myFFT(QVector<double> *input, fftw_complex *out, int N);
    static void myIFFT(fftw_complex *input, QVector<double> *out, int N);
    static double linearInterpolation(double x1, double y1, double x2, double y2, double yTarget);
    static double findFWHM(QVector<double> *data, QVector<double> *xValues);

    // static void convolution_fftw(const coder::array<double, 2U> &input,
    //                              const coder::array<double, 2U> &kernel,
    //                              coder::array<double, 2U> &L_out);
private:
    static QMutex mutex;
};

#endif // MYMATH_H
