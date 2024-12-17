#include "fizeauifgenerator.h"
#include "utils/logger.h"
#include "matlab/FizeauInstrument.h"
#include "model/constantstorage.h"
#include "model/constantmap.h"
#include "model/frequencedatagenerator.h"
#include "utils/mymath.h"

FizeauIFGenerator::FizeauIFGenerator() {}

QVector<QVector<double> *> *FizeauIFGenerator::generateFizeauIFData()
{
    Singleton<Logger>::getInstance()->logMessage("Generate Fresnel interferometer data.",
                                                 Logger::Log);

    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);

    double lambda = constantStorage->getConstant(constantMap->getConstantName(2, 0)).toDouble();
    double Fizeau_plate = constantStorage->getConstant(constantMap->getConstantName(2, 1)).toDouble();
    double Fizeau_angle = constantStorage->getConstant(constantMap->getConstantName(2, 2)).toDouble();
    double Fizeau_index = constantStorage->getConstant(constantMap->getConstantName(2, 3)).toDouble();
    double Fizeau_albedo = constantStorage->getConstant(constantMap->getConstantName(2, 4)).toDouble();

    double channel_number = constantStorage->getConstant(constantMap->getConstantName(3, 0)).toDouble();
    double channel_width = constantStorage->getConstant(constantMap->getConstantName(3, 1)).toDouble();
    double channel_space = constantStorage->getConstant(constantMap->getConstantName(3, 2)).toDouble();

    double PMT_length = channel_number * channel_width + (channel_number - 1) * channel_space;
    double alpha_cal = atan(lambda / 2 / PMT_length);

    double *Fizeau_FSR = new double();
    coder::array<double, 2U> Fizeau_spectrum;
    QVector<double> *RF = FrequenceDataGenerator::generateRelativeFrequenceData();
    coder::array<double, 2U> Fizeau_frequency;
    coder::array<double, 2U> frequency;

    MyMath::convertQVectorToArray(RF, frequency);

    if (Fizeau_angle > alpha_cal)
    {
        Singleton<Logger>::getInstance()
            ->logMessage("The angle of the Fresnel interferometer is greater than the calculated "
                         "angle and cannot be calculated",
                         Logger::Warning);
        return nullptr;
    }

    //[FSR, FizeauFunction, ~] = FizeauInstrument(alpha_cal, Frequency, lambda, Fizeau_plate, Fizeau_angle, Fizeau_index, Fizeau_albedo);
    FizeauInstrument(alpha_cal, frequency, lambda, Fizeau_plate, Fizeau_angle, Fizeau_index, Fizeau_albedo, Fizeau_FSR, Fizeau_spectrum, Fizeau_frequency);

    Singleton<Logger>::getInstance()->logMessage("Fizeau interferometer data generation completed",
                                                 Logger::Log);
    // 打印FSR
    Singleton<Logger>::getInstance()->logMessage("FSR: " + QString::number(*Fizeau_FSR), Logger::Log);

    // 将Fizeau_spectrum和Fizeau_frequency转换为QVector<QVector<double> *>
    QVector<QVector<double> *> *result = new QVector<QVector<double> *>();
    QVector<double> *spectrum_vector;
    QVector<double> *frequency_vector;

    spectrum_vector = MyMath::convertArrayToQVector(Fizeau_spectrum);
    frequency_vector = MyMath::convertArrayToQVector(Fizeau_frequency);

    // Fizeau_spectrum = FizeauFunction ./ polyarea(Frequency, FizeauFunction); %Area normalization
    // double area = MyMath::polyarea(*frequency_vector, *spectrum_vector);
    // double area = MyMath::polyarea(*RF, *spectrum_vector);
    // for (int i = 0; i < Fizeau_spectrum.size(1); ++i)
    // {
    //     // spectrum_vector[i] = spectrum_vector[i] / area;
    //     (*spectrum_vector)[i] = (*spectrum_vector)[i] / area;
    // }

    result->append(frequency_vector);
    result->append(spectrum_vector);

    // Singleton<Logger>::getInstance()->logMessage("频率最大值: " + QString::number(max) + "，索引: " + QString::number(index), Logger::Log);
    // // 输出spectrum_vector的最大值和索引
    // double max = 0;
    // int index = 0;
    // for (int i = 0; i < spectrum_vector->size(); ++i)
    // {
    //     if ((*spectrum_vector)[i] > max)
    //     {
    //         max = (*spectrum_vector)[i];
    //         index = i;
    //     }
    // }
    // Singleton<Logger>::getInstance()->logMessage("最大值: " + QString::number(max) + "，索引: " + QString::number(index), Logger::Log);

    delete RF;

    return result;
}

QVector<double> *FizeauIFGenerator::Fizeau_Polyarea = nullptr;

QVector<double> *FizeauIFGenerator::getAfterFizeauSpectrumData(QVector<double> *spectrum_data)
{
    QVector<QVector<double> *> *Fizeau_IF;
    QVector<double> *xData;
    QVector<double> *result;
    // 拷贝fizeau_IF，再对其进行面积归一化
    if (Fizeau_Polyarea == nullptr)
    {
        Fizeau_IF = generateFizeauIFData();
        Fizeau_Polyarea = new QVector<double>(*Fizeau_IF->at(1));
        // 归一化
        xData = FrequenceDataGenerator::generateRelativeFrequenceData();
        double area = MyMath::polyarea(*xData, *Fizeau_Polyarea);
        for (int i = 0; i < Fizeau_Polyarea->size(); ++i)
        {
            (*Fizeau_Polyarea)[i] = (*Fizeau_Polyarea)[i] / area;
        }

        result = MyMath::convolution(spectrum_data, Fizeau_Polyarea);

        // QVector<QVector<double> *> *resultVector = new QVector<QVector<double> *>();
        // resultVector->append(xData);
        // resultVector->append(result);

        delete xData;
        delete Fizeau_IF->at(0);
        delete Fizeau_IF->at(1);
        delete Fizeau_IF;
    }
    else if (Fizeau_Polyarea != nullptr)
    {
        result = MyMath::convolution(spectrum_data, Fizeau_Polyarea);
    }

    delete spectrum_data;

    return result;
}

QVector<QVector<double> *> *FizeauIFGenerator::calculateSpectrumAfterFizeau(QVector<double> *fizeau_IF)
{
    Singleton<Logger>::getInstance()
        ->logMessage("Calculate the spectrum after passing through a Fizeau interferometer",
                     Logger::Log);
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);

    QSharedPointer<QCPGraphDataContainer> dataContainer;
    //  constantStorage->getConstant(constantMap->getConstantName(5, 11))
    dataContainer = constantStorage->getConstant(constantMap->getConstantName(5, 11)).value<QSharedPointer<QCPGraphDataContainer>>();

    if (dataContainer.isNull())
    {
        Singleton<Logger>::getInstance()->logMessage(
            "The underwater spectral data is empty! Please provide underwater spectral data!",
            Logger::Warning);
        return nullptr;
    }

    QVector<double> *xData = new QVector<double>();
    QVector<double> *yData = new QVector<double>();
    constantStorage->convertQSharedPointerToQVector(dataContainer, xData, yData);
    QVector<double> *fizeau_IF_copy = new QVector<double>(*fizeau_IF);
    // if (Fizeau_Polyarea == nullptr)
    // {
    //     fizeau_IF_copy = new QVector<double>(*fizeau_IF);
    // }
    // else
    // {
    //     fizeau_IF_copy = new QVector<double>(*Fizeau_Polyarea);
    // }

    double area = MyMath::polyarea(*xData, *fizeau_IF_copy);
    for (int i = 0; i < fizeau_IF_copy->size(); ++i)
    {
        (*fizeau_IF_copy)[i] = (*fizeau_IF_copy)[i] / area;
    }

    if (Fizeau_Polyarea == nullptr)
    {
        Fizeau_Polyarea = new QVector<double>(*fizeau_IF_copy);
    }
    else
    {
        delete Fizeau_Polyarea;
        Fizeau_Polyarea = new QVector<double>(*fizeau_IF_copy);
    }

    QVector<double> *result = MyMath::convolution(yData, fizeau_IF_copy);

    QVector<QVector<double> *> *resultVector = new QVector<QVector<double> *>();
    resultVector->append(xData);
    resultVector->append(result);

    delete yData;
    delete fizeau_IF_copy;

    return resultVector;
}
