#include "model/pagedatagenerator.h"
#include "model/fizeaudatagenerator.h"
#include "model/frequencedatagenerator.h"
#include "model/laserdatagenerator.h"
#include "model/spectrumdatagenerator.h"
#include "model/constantstorage.h"
#include "model/constantmap.h"
#include "module/filebrowser.h"
#include "utils/logger.h"
#include "model/underwaterspectrumdatagenerator.h"
#include "model/fizeauifgenerator.h"

PageDataGenerator::PageDataGenerator(QObject *parent)
    : QObject(parent)
{
    Singleton<ConstantMap>::getInstance();
    Singleton<ConstantStorage>::getInstance(nullptr);
    // 注册元类型
    qRegisterMetaType<QSharedPointer<QCPGraphDataContainer>>("QSharedPointer<QCPGraphDataContainer>");
}

PageDataGenerator::~PageDataGenerator() {}

QVector<double> *PageDataGenerator::generateData(DataType dataType)
{
    QVector<double> *data = nullptr;
    switch (dataType)
    {
    case DataType::Frequence:
        data = FrequenceDataGenerator::generateRelativeFrequenceData();
        break;
    case DataType::Laser:
        data = LaserDataGenerator::generateLaserData();
        break;
    case DataType::MieScattering:
        data = SpectrumDataGenerator::generateMieScatteringData();
        break;
    case DataType::BriScattering:
        data = SpectrumDataGenerator::generateBriScatteringData();
        break;
    case DataType::RayScattering:
        data = SpectrumDataGenerator::generateRayScatteringData();
        break;
    // case DataType::UnderwaterScattering:
    //     UnderwaterScatteringDataGenerator::generateUnderwaterScatteringData(data);
    //     break;
    // case DataType::FizeauInstrument:
    //     data = FizeauDataGenerator::generateFizeauData();
    //     break;
    // case DataType::FizeauSpectra:
    //     data = FizeauDataGenerator::generateSpectrumAfterFizeau();
    //     break;
    // case DataType::PMTFrequency:
    //     PMTFrequencyDataGenerator::generatePMTFrequencyData(data);
    //     break;
    // case DataType::PMTEnergy:
    //     PMTEnergyDataGenerator::generatePMTEnergyData(data);
    //     break;
    // Add more cases for other page types
    default:
        data = nullptr;
        break;
    }

    return data;
}

void PageDataGenerator::generatePairOfData(int page_index)
{
    QVector<QVector<double> *> *xDataVector;
    QVector<QVector<double> *> *yDataVector;
    xDataVector = new QVector<QVector<double> *>;
    yDataVector = new QVector<QVector<double> *>;
    QVector<QVector<double> *> *SpectrumData;
    // QVector<double> *result;
    QVector<QVector<double> *> *laserLineWidthEffectData;

    switch (page_index)
    {
    case 1:
        xDataVector->append(generateData(DataType::Frequence));
        yDataVector->append(generateData(DataType::Laser));
        emit dataGenerated(xDataVector, yDataVector, 1);
        break;
    case 2:
        // xDataVector->append(generateData(DataType::Frequence));
        // yDataVector->append(generateData(DataType::BriScattering));
        // yDataVector->append(generateData(DataType::RayScattering));
        // yDataVector->append(generateData(DataType::MieScattering));

        SpectrumData = SpectrumDataGenerator::generateSpectrumDataByMatlabCode();
        xDataVector->append(generateData(DataType::Frequence));
        yDataVector->append(SpectrumData->at(0));
        yDataVector->append(SpectrumData->at(1));
        yDataVector->append(SpectrumData->at(2));
        emit dataGenerated(xDataVector, yDataVector, 3);
        break;
    case 3:
        // xDataVector->append(
        //     PageDataGenerator::generateData(PageDataGenerator::DataType::Frequence, inputDataList));
        // yDataVector->append(
        //     PageDataGenerator::generateData(PageDataGenerator::DataType::MieScattering,
        //                                     inputDataList));
        laserLineWidthEffectData = SpectrumDataGenerator::generateLaserLineWidthEffectData();
        if (laserLineWidthEffectData == nullptr || laserLineWidthEffectData->size() != 4)
        {
            Singleton<Logger>::getInstance()->logMessage("激光线宽对三种散射谱的影响数据生成失败！", Logger::Warning);
            return;
        }
        xDataVector->append(generateData(DataType::Frequence));
        yDataVector->append(laserLineWidthEffectData->at(0));
        yDataVector->append(laserLineWidthEffectData->at(1));
        yDataVector->append(laserLineWidthEffectData->at(2));
        yDataVector->append(laserLineWidthEffectData->at(3));
        emit dataGenerated(xDataVector, yDataVector, 4);
        break;
    case 4:
        // xDataVector->append(generateData(DataType::Frequence));
        // yDataVector->append(generateData(DataType::FizeauInstrument));
        // yDataVector->append(generateData(DataType::FizeauSpectra));
        // emit dataGenerated(xDataVector, yDataVector, 2);
        laserLineWidthEffectData = UnderWaterSpectrumDataGenerator::generateUnderWaterSpectrumData();
        if (laserLineWidthEffectData == nullptr || laserLineWidthEffectData->size() != 4)
        {
            Singleton<Logger>::getInstance()->logMessage("激光线宽对三种散射谱的影响数据生成失败！", Logger::Warning);
            return;
        }
        xDataVector->append(generateData(DataType::Frequence));
        yDataVector->append(laserLineWidthEffectData->at(0));
        yDataVector->append(laserLineWidthEffectData->at(1));
        yDataVector->append(laserLineWidthEffectData->at(2));
        yDataVector->append(laserLineWidthEffectData->at(3));
        emit dataGenerated(xDataVector, yDataVector, 4);

        break;
    case 5:
        laserLineWidthEffectData = FizeauIFGenerator::generateFizeauIFData();
        xDataVector->append(laserLineWidthEffectData->at(0));
        yDataVector->append(laserLineWidthEffectData->at(1));
        laserLineWidthEffectData = FizeauIFGenerator::calculateSpectrumAfterFizeau(laserLineWidthEffectData->at(1));
        xDataVector->append(laserLineWidthEffectData->at(0));
        yDataVector->append(laserLineWidthEffectData->at(1));
        emit dataGenerated(xDataVector, yDataVector, 2);
        break;
        // case 6:
        //     xDataVector->append(
        //         PageDataGenerator::generateData(PageDataGenerator::DataType::PMTFrequency,
        //                                         inputDataList));
        //     yDataVector->append(
        //         PageDataGenerator::generateData(PageDataGenerator::DataType::PMTEnergy,
        //                                         inputDataList));
        //     emit dataGenerated(xDataVector, yDataVector, 1);
        //     break;
    }
    // 释放InputDataListManager内存
    // delete inputDataList;
}

void PageDataGenerator::storeRuntimeDataByIndex(QSharedPointer<QCPGraphDataContainer> dataContainer, const int page_index, const int curve_index)
{
    switch (page_index)
    {
    case 0:
        Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(5, curve_index), QVariant::fromValue(dataContainer));
        // 打印底层存储地址
        // qDebug() << "dataContainer: " << dataContainer.data();
        break;
    case 1:
        Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(5, 1 + curve_index), QVariant::fromValue(dataContainer));
        break;

    case 2:
        Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(5, 4 + curve_index), QVariant::fromValue(dataContainer));
        break;

    case 3:
        Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(5, 8 + curve_index), QVariant::fromValue(dataContainer));
        break;

    case 4:
        Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(5, 12 + curve_index), QVariant::fromValue(dataContainer));
        break;
    }
}

void PageDataGenerator::storeConstantByGroupIndex(int index)
{
    FileBrowser fileBrowser;
    QStringList filePaths = fileBrowser.saveJsonFilesDialog(QDir::currentPath(), false);
    if (filePaths.isEmpty())
    {
        Singleton<Logger>::getInstance()->logMessage("未选择文件路径！", Logger::Warning);
        return;
    }
    Singleton<ConstantStorage>::getInstance(nullptr)->savePageConstantToJsonFile(index, filePaths);
}

void PageDataGenerator::storeAllConstant()
{
    FileBrowser fileBrowser;
    QStringList filePaths = fileBrowser.saveJsonFilesDialog(QDir::currentPath(), false);
    if (filePaths.isEmpty())
    {
        Singleton<Logger>::getInstance()->logMessage("未选择文件路径！", Logger::Warning);
        return;
    }
    Singleton<ConstantStorage>::getInstance(nullptr)->saveAllPageConstantToJsonFile(filePaths);
}

void PageDataGenerator::storeRuntimeDataByGroupIndex(int index)
{
    FileBrowser fileBrowser;
    QStringList filePaths = fileBrowser.saveCsvFilesDialog(QDir::currentPath(), false);
    if (filePaths.isEmpty())
    {
        Singleton<Logger>::getInstance()->logMessage("未选择文件路径！", Logger::Warning);
        return;
    }
    Singleton<ConstantStorage>::getInstance(nullptr)->savePageRuntimeDataToCSVFile(index, filePaths);
}

void PageDataGenerator::storeAllRuntimeData()
{
    FileBrowser fileBrowser;
    QStringList filePaths = fileBrowser.saveCsvFilesDialog(QDir::currentPath(), true);
    if (filePaths.isEmpty())
    {
        Singleton<Logger>::getInstance()->logMessage("未选择文件路径！", Logger::Warning);
        return;
    }
    Singleton<ConstantStorage>::getInstance(nullptr)->saveAllPageRuntimeDataToCSVFile(filePaths);
}

void PageDataGenerator::importConstantByGroupIndex(int index)
{
    FileBrowser fileBrowser;
    QStringList filePaths = fileBrowser.openJsonFilesDialog(QDir::currentPath(), false);
    if (filePaths.isEmpty())
    {
        Singleton<Logger>::getInstance()->logMessage("未选择文件路径！", Logger::Warning);
        return;
    }
    Singleton<ConstantStorage>::getInstance(nullptr)->importPageConstantFromJsonFile(filePaths);
}

void PageDataGenerator::importAllConstant()
{
    FileBrowser fileBrowser;
    QStringList filePaths = fileBrowser.openJsonFilesDialog(QDir::currentPath(), false);
    if (filePaths.isEmpty())
    {
        Singleton<Logger>::getInstance()->logMessage("未选择文件路径！", Logger::Warning);
        return;
    }
    Singleton<ConstantStorage>::getInstance(nullptr)->importAllPageConstantFromJsonFile(filePaths);
}

void PageDataGenerator::importRuntimeDataByGroupIndex(int index)
{
    QVector<QVector<QVector<double> *> *> *xDataVector;
    QVector<QVector<QVector<double> *> *> *yDataVector;
    xDataVector = new QVector<QVector<QVector<double> *> *>;
    yDataVector = new QVector<QVector<QVector<double> *> *>;
    xDataVector->append(new QVector<QVector<double> *>);
    yDataVector->append(new QVector<QVector<double> *>);
    FileBrowser fileBrowser;
    QStringList filePaths = fileBrowser.openCsvFilesDialog(QDir::currentPath(), false);
    if (filePaths.isEmpty())
    {
        Singleton<Logger>::getInstance()->logMessage("未选择文件路径！", Logger::Warning);
        return;
    }
    Singleton<ConstantStorage>::getInstance(nullptr)->importPageRuntimeDataFromCSVFile(index, filePaths, xDataVector->at(0), yDataVector->at(0));
    emit importConstantCompleted(index, xDataVector, yDataVector);
}

void PageDataGenerator::importAllRuntimeData()
{
    QVector<QVector<QVector<double> *> *> *xDataVector;
    QVector<QVector<QVector<double> *> *> *yDataVector;
    xDataVector = new QVector<QVector<QVector<double> *> *>;
    yDataVector = new QVector<QVector<QVector<double> *> *>;
    FileBrowser fileBrowser;
    QStringList filePaths = fileBrowser.openCsvFilesDialog(QDir::currentPath(), true);
    if (filePaths.isEmpty())
    {
        Singleton<Logger>::getInstance()->logMessage("未选择文件路径！", Logger::Warning);
        return;
    }

    // 提示选择了多少个文件
    Singleton<Logger>::getInstance()->logMessage("已选择" + QString::number(filePaths.size()) + "个文件！", Logger::Info);

    // 给xDataVector和yDataVector内部的QVector<QVector<double> *>分配filePaths.size()的内存
    for (int i = 0; i < filePaths.size(); ++i)
    {
        xDataVector->append(new QVector<QVector<double> *>);
        yDataVector->append(new QVector<QVector<double> *>);
    }

    Singleton<ConstantStorage>::getInstance(nullptr)->importAllPageRuntimeDataFromCSVFile(filePaths, xDataVector, yDataVector);
    emit importConstantCompleted(0, xDataVector, yDataVector);
}
