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
#include "model/pmtreceptiondatagenerator.h"
#include "model/retrievalthread.h"
#include "model/dynamicpagedatageneratorthread.h"

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
    case 4:
        laserLineWidthEffectData = FizeauIFGenerator::generateFizeauIFData();
        xDataVector->append(laserLineWidthEffectData->at(0));
        yDataVector->append(laserLineWidthEffectData->at(1));
        laserLineWidthEffectData = FizeauIFGenerator::calculateSpectrumAfterFizeau(laserLineWidthEffectData->at(1));
        // xDataVector->append(laserLineWidthEffectData->at(0));
        yDataVector->append(laserLineWidthEffectData->at(1));
        // laserLineWidthEffectData = PMTReceptionDataGenerator::receiveSpectrumAfterPMT(laserLineWidthEffectData->at(1));
        // xDataVector->append(laserLineWidthEffectData->at(0));
        // yDataVector->append(laserLineWidthEffectData->at(1));
        // laserLineWidthEffectData = PMTReceptionDataGenerator::receiveSpectrumAfterPMT(laserLineWidthEffectData->at(1));
        // xDataVector->append(laserLineWidthEffectData->at(0));
        // yDataVector->append(laserLineWidthEffectData->at(1));
        emit dataGenerated(xDataVector, yDataVector, 2);
        break;
    case 5:
        laserLineWidthEffectData = PMTReceptionDataGenerator::generatePMTReceptionData();
        // 从constantstorage中拿到SpectrumAfterFizeau的数据
        // PMTReceptionDataGenerator::retrievalFormPMT();

        xDataVector->append(laserLineWidthEffectData->at(0));
        xDataVector->append(laserLineWidthEffectData->at(2));
        yDataVector->append(laserLineWidthEffectData->at(1));
        yDataVector->append(laserLineWidthEffectData->at(3));
        emit dataGenerated(xDataVector, yDataVector, 2);
    }
    // 释放InputDataListManager内存
    // delete inputDataList;
}

void PageDataGenerator::generateDynamicData(int index)
{
    TaskRunner *object = TaskRunner::runTask<DynamicPageDataGeneratorThread>(index);
    connect(object, &TaskRunner::taskCompleted, this, &PageDataGenerator::handleTaskCompletedSlot);
}

// 产生动态的行为，例如开始拟合，模型反演等，注意无显示曲线
void PageDataGenerator::generateDynamicAction(int index)
{
    switch (index)
    {
    case 0:
        // PMTReceptionDataGenerator::retrievalFormPMT();
        // emit actionGenerateFinished();
        TaskRunner *object = TaskRunner::runTask<RetrievalThread>(PMTReceptionDataGenerator::retrievalFormPMT);
        connect(object, &TaskRunner::taskCompleted, this, &PageDataGenerator::handleTaskCompletedSlot);
        break;
    }
}

int PageDataGenerator::captureImageData(int index, QRect captureRect)
{
    TaskRunner *object = TaskRunner::runTask<ScreenCaptureTask>(index, captureRect);
    connect(object, &TaskRunner::taskCompleted, this, &PageDataGenerator::handleTaskCompletedSlot);
    return 1;
}

int PageDataGenerator::getImageData(int index, QPixmap *pixmap)
{
    QPixmap *pixmapPtr = Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(Singleton<ConstantMap>::getInstance()->getConstantName(8, index)).value<QPixmap *>();

    if (pixmapPtr == nullptr)
    {
        return 0; // 获取图片失败
    }

    // if (pixmap != nullptr)
    // {
    //     delete pixmap;
    // }

    *pixmap = *pixmapPtr;

    return 1; // 获取图片成功
}

int PageDataGenerator::getInfoData(int index, QMap<QString, QVariant> *info)
{
    // 遍历constantstorage，拿到对应页面所有的信息
    QMap<QString, QVariant> *infoPtr = new QMap<QString, QVariant>();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();

    switch (index)
    {
    case 0:
        for (int i = 0; i < 6; i++)
        {
            infoPtr->insert(constantMap->getConstantName(0, i), constantStorage->getConstant(constantMap->getConstantName(0, i)));
        }
        break;
    case 1:
        for (int i = 0; i < 6; i++)
        {
            infoPtr->insert(constantMap->getConstantName(1, i), constantStorage->getConstant(constantMap->getConstantName(1, i)));
        }
        break;
    case 2:
        for (int i = 0; i < 9; i++)
        {
            infoPtr->insert(constantMap->getConstantName(7, i), constantStorage->getConstant(constantMap->getConstantName(7, i)));
        }
        break;
    case 3:
        for (int i = 0; i < 5; i++)
        {
            infoPtr->insert(constantMap->getConstantName(2, i), constantStorage->getConstant(constantMap->getConstantName(2, i)));
        }
        break;
    case 4:
        for (int i = 0; i < 3; i++)
        {
            infoPtr->insert(constantMap->getConstantName(3, i), constantStorage->getConstant(constantMap->getConstantName(3, i)));
        }
        break;
    }

    if (infoPtr == nullptr)
    {
        return 0; // 获取信息失败
    }

    *info = *infoPtr;

    return 1; // 获取信息成功
}

void PageDataGenerator::handleTaskCompletedSlot(QString taskName, QVariantList *args)
{
    if (taskName.contains("ScreenCaptureTask"))
    {
        int index = taskName.right(1).toInt(); // 用来区分不同的截图任务
        if (args->at(0).toInt() == 1)
        {
            return;
        }
        else if (args->at(0).toInt() == 0)
        {
            Singleton<Logger>::getInstance()->logMessage("截图失败！", Logger::Warning);
        }
        else if (args->at(0).toInt() == -1)
        {
            Singleton<Logger>::getInstance()->logMessage("无法获取屏幕！", Logger::Warning);
        }
    }
    else if (taskName.contains("RetrievalThread"))
    {
        // if (args->at(0).toInt() == 1)
        // {
        //     Singleton<Logger>::getInstance()->logMessage("PMT反演成功！", Logger::Info);
        // }
        // else if (args->at(0).toInt() == 0)
        // {
        //     Singleton<Logger>::getInstance()->logMessage("PMT反演失败！", Logger::Warning);
        // }
        emit retrievalCompleted(args);
    }
    else if (taskName.contains("DynamicPageDataGeneratorThread"))
    {
        qDebug() << "args->at(0).toInt():" << args->at(0).toInt();
        if (args->at(0).toInt() == 0)
        {
            // emit dynamicDataGenerated(xDataVector, yDataVector, i, QString("受激光拓宽的散射谱"), legendList); // i 表示第几面的曲线
        }
        else if (args->at(0).toInt() == 1)
        {
            // Singleton<Logger>::getInstance()->logMessage("数据生成成功！", Logger::Info);
            emit dynamicDataGenerated(args->at(1).value<QVector<QVector<double> *> *>(), args->at(2).value<QVector<QVector<double> *> *>(), args->at(3).toInt(), args->at(4).toString(), args->at(5).toStringList(), args->at(6).toStringList());
        }
        else if (args->at(0).toInt() == 2)
        {
            // Singleton<Logger>::getInstance()->logMessage("全部数据生成成功！", Logger::Info);
            emit dataGenerateFinished();
        }
    }
}

void PageDataGenerator::storeRuntimeDataByIndex(QSharedPointer<QCPGraphDataContainer> dataContainer, const int page_index, const int curve_index, int page_type)
{
    if (page_type == 0)
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
            Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(5, 8 + curve_index), QVariant::fromValue(dataContainer));
            break;

        case 3:
            Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(5, 12 + curve_index), QVariant::fromValue(dataContainer));
            break;

        case 4:
            // Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(5, 14 + curve_index), QVariant::fromValue(dataContainer));
            break;
        case 5:
            // Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(5, 14 + curve_index), QVariant::fromValue(dataContainer));
            break;
        }
    }
    else if (page_type == 1) // 在扩展页面中，page_type表示pageObjectId，curve_num是整个pageObject的所有页面的curve的索引
    {
        switch (page_index)
        {
        case 0:
            Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(Singleton<ConstantMap>::getInstance()->getConstantName(5, 4 + curve_index), QVariant::fromValue(dataContainer));
            break;
        }
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
