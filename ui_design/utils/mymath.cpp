#include "mymath.h"
#include <complex>
#include <stdlib.h>
#include <stdio.h>

QMutex MyMath::mutex;

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

    delete temp_out;
    fftw_free(inputComplex);
    fftw_free(kernelComplex);
    fftw_free(outputComplex);

    return L_out;
}

// 计算N点FFT函数
void MyMath::myFFT(QVector<double> *input, fftw_complex *out, int N)
{
    mutex.lock();
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
    mutex.unlock();
}

// 计算N点IFFT函数
void MyMath::myIFFT(fftw_complex *in, QVector<double> *out, int N)
{
    mutex.lock();
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
    mutex.unlock();
}

double MyMath::linearInterpolation(double x1, double y1, double x2, double y2, double yTarget)
{
    return x1 + (yTarget - y1) * (x2 - x1) / (y2 - y1);
}

double MyMath::findFWHM(QVector<double> *data, QVector<double> *xValues)
{
    const double halfMax = 0.5;
    std::vector<int> col_LS;
    int size = data->size();

    // 寻找 LS 在 (0.499, 0.519) 范围内的点
    for (int i = 0; i < size; ++i)
    {
        if ((*data)[i] > 0.3 && (*data)[i] < 0.7)
        {
            col_LS.push_back(i);
        }
    }

    if (col_LS.size() < 2)
    {
        // Singleton<Logger>::getInstance()->logMessage(
        //     "Setting the laser_width within this frequency range cannot calculate width_laser. "
        //     "Please modify the frequency range or the size of laser_width",
        //     Logger::Warning);
        return -1;
    }

    // 找到半高处左侧的点
    int leftIndex = -1;
    int rightIndex = -1;

    for (size_t i = 0; i < col_LS.size() - 1; ++i)
    {
        int currentIndex = col_LS[i];
        int nextIndex = col_LS[i + 1];

        if ((*data)[currentIndex] < halfMax && (*data)[nextIndex] > halfMax)
        {
            leftIndex = currentIndex;
            rightIndex = nextIndex;
            break;
        }
    }

    if (leftIndex == -1 || rightIndex == -1)
    {
        // Singleton<Logger>::getInstance()->logMessage("Unable to find left and right indices for interpolation.", Logger::Warning);
        return -1;
    }

    // 插值求左侧半高宽点
    double leftHalfMaxX = linearInterpolation((*xValues)[leftIndex], (*data)[leftIndex],
                                              (*xValues)[rightIndex], (*data)[rightIndex],
                                              halfMax);

    // 找到半高处右侧的点
    leftIndex = -1;
    rightIndex = -1;

    for (size_t i = col_LS.size() - 1; i > 0; --i)
    {
        int currentIndex = col_LS[i];
        int prevIndex = col_LS[i - 1];

        if ((*data)[currentIndex] < halfMax && (*data)[prevIndex] > halfMax)
        {
            leftIndex = prevIndex;
            rightIndex = currentIndex;
            break;
        }
    }

    if (leftIndex == -1 || rightIndex == -1)
    {
        // Singleton<Logger>::getInstance()->logMessage("Unable to find left and right indices for interpolation.", Logger::Warning);
        return -1;
    }

    // 插值求右侧半高宽点
    double rightHalfMaxX = linearInterpolation((*xValues)[leftIndex], (*data)[leftIndex],
                                               (*xValues)[rightIndex], (*data)[rightIndex],
                                               halfMax);

    // 计算并返回半高宽
    return rightHalfMaxX - leftHalfMaxX;
}