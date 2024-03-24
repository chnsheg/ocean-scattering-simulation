#include "mymath.h"
#include <complex>
#include <stdlib.h>
#include <stdio.h>

MyMath::MyMath() {}

// 将 coder::array 转换为 QVector
QVector<double> *MyMath::convertArrayToQVector(const coder::array<double, 2U> &array)
{
    QVector<double> *vector = new QVector<double>(array.size(1));
    for (int i = 0; i < array.size(1); i++)
    {
        (*vector)[i] = array[i];
    }
    return vector;
}

// 将多维的coder::array 转换为 QVector<QVector<double> *>*
QVector<QVector<double> *> *MyMath::convertMultiArrayToQVector(const coder::array<double, 2U> &array)
{
    QVector<QVector<double> *> *vector = new QVector<QVector<double> *>();
    for (int i = 0; i < array.size(0); i++)
    {
        QVector<double> *temp = new QVector<double>(array.size(1));
        for (int j = 0; j < array.size(1); j++)
        {
            (*temp)[j] = array.at(i, j);
        }
        vector->append(temp);
    }
    return vector;
}

// 将 QVector 转换为 coder::array
void MyMath::convertQVectorToArray(const QVector<double> *vector, coder::array<double, 2U> &array)
{
    array.set_size(1, vector->size());
    for (int i = 0; i < vector->size(); i++)
    {
        array[i] = vector->at(i);
    }
}

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

QVector<double> *MyMath::convolution(QVector<double> *input, QVector<double> *kernel)
{
    int N = input->size() + kernel->size() - 1;
    QVector<double> *L_out = new QVector<double>(input->size());
    QVector<double> *temp_out = new QVector<double>(N);
    fftw_complex *inputComplex = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    fftw_complex *kernelComplex = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    fftw_complex *outputComplex = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

    myFFT(input, inputComplex, N);
    myFFT(kernel, kernelComplex, N);

    for (int i = 0; i < N; ++i)
    {
        outputComplex[i][0] = (inputComplex[i][0] * kernelComplex[i][0] - inputComplex[i][1] * kernelComplex[i][1]);
        outputComplex[i][1] = (inputComplex[i][0] * kernelComplex[i][1] + inputComplex[i][1] * kernelComplex[i][0]);
    }

    myIFFT(outputComplex, temp_out, N);

    int startIndex = (N - input->size()) / 2;
    for (int i = 0; i < input->size(); ++i)
    {
        (*L_out)[i] = temp_out->at(startIndex + i) / N;
    }

    return L_out;
}

// 计算N点FFT函数
void MyMath::myFFT(QVector<double> *input, fftw_complex *out, int N)
{
    double *in;
    fftw_plan plan;

    // 分配内存
    in = (double *)fftw_malloc(sizeof(double) * N);

    // 初始化输入数据
    for (int i = 0; i < N; ++i)
    {
        if (i < input->size())
        {
            in[i] = input->at(i);
        }
        else
        {
            in[i] = 0.0;
        }
    }

    // 创建FFT计划
    plan = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);

    // 执行FFT
    fftw_execute(plan);

    // 释放内存
    fftw_destroy_plan(plan);
    fftw_free(in);
}

// 计算N点IFFT函数
void MyMath::myIFFT(fftw_complex *in, QVector<double> *out, int N)
{
    double *result;
    fftw_plan plan;

    // 分配内存
    result = (double *)fftw_malloc(sizeof(double) * N);

    // 创建IFFT计划
    plan = fftw_plan_dft_c2r_1d(N, in, result, FFTW_ESTIMATE);

    // 执行IFFT
    fftw_execute(plan);

    // 将结果放回QVector
    for (int i = 0; i < N; ++i)
    {
        (*out)[i] = result[i];
    }

    // 释放内存
    fftw_destroy_plan(plan);
    fftw_free(result);
}