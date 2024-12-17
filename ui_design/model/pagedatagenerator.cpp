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
    qRegisterMetaType<QVector<QVector<double> *> *>("QVector<QVector<double> *> *");

    receivedDataContainer = nullptr;
}

PageDataGenerator::~PageDataGenerator() {}

/**
 * @brief 生成激光和光谱数据
 */
QVector<double> *PageDataGenerator::generateData(DataType dataType)
{
    QVector<double> *data = nullptr;
    switch (dataType)
    {
    case DataType::Frequence:
        data = FrequenceDataGenerator::generateRelativeFrequenceData(); // 注意这里的数据是相对频率
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

    default:
        data = nullptr;
        break;
    }

    return data;
}

/**
 * @brief 总的数据调用和返回的接口，直接返回绘图的容器
 * @param page_index 页面索引
 */
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
            Singleton<Logger>::getInstance()
                ->logMessage("The impact of laser linewidth on three types of scattering spectra "
                             "data generation failed!",
                             Logger::Warning);
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
        xDataVector->append(laserLineWidthEffectData->at(4)); // 未加噪信号
        yDataVector->append(laserLineWidthEffectData->at(1));
        yDataVector->append(laserLineWidthEffectData->at(3));
        yDataVector->append(laserLineWidthEffectData->at(5)); // 未加噪信号
        // caculateRetrievalErrorByDepth();
        emit dataGenerated(xDataVector, yDataVector, 3);
    }
    // 释放InputDataListManager内存
    // delete inputDataList;
}

/**
 * @brief 新建一个线程生成动态界面的数据
 * @param index 页面索引
 */
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

void PageDataGenerator::caculateRetrievalErrorByDepth()
{
    // 先获取各深度下的散射光子数和信噪比
    QVector<QVector<double> *> *scatteringDataContainer = UnderWaterSpectrumDataGenerator::UnderWaterSpectrumDataGenerator::getNsByDepthData();
    QVector<double> *zDepth = scatteringDataContainer->at(0);
    QVector<double> *N_Bri = scatteringDataContainer->at(1);
    // QVector<double>* N_Mie = scatteringDataContainer->at(2);
    QVector<double> *N_Rayleigh = scatteringDataContainer->at(2);
    QVector<double> *SNR = scatteringDataContainer->at(3);
    // TaskRunner *object = TaskRunner::runTask<RetrievalThread>(PMTReceptionDataGenerator::retrievalFormPMT);
    // 分配zDepth大小的内存，每个线程计算的结果对应replace掉DataContainer中的一个位置
    receivedDataContainer = new QVector<QVector<double> *>(zDepth->size());

    int thread_type = 1;

    if (receivedDataContainer->size() > 1)
    {
        if (zDepth->at(0) == zDepth->at(1))
        {
            thread_type = 2; // 同一深度下的重复反演
        }
    }

    // 创建zDepth大小的线程，每个线程计算一个深度的误差
    for (int i = 0; i < zDepth->size(); i++)
    {
        if (zDepth->size() <= 0)
        {
            delete receivedDataContainer;
            delete scatteringDataContainer;
            return;
        }

        qDebug() << "zDepth:" << zDepth->at(i);
        TaskRunner *object = TaskRunner::runTask<RetrievalThread>(PageDataGenerator::receiveSystemCallbackFunc, thread_type, N_Bri->at(i), N_Rayleigh->at(i), SNR->at(i), receivedDataContainer, i, zDepth->at(i));
        connect(object, &TaskRunner::taskCompleted, this, &PageDataGenerator::handleTaskCompletedSlot);
    }
}

/**
 * @brief 接收系统的回调函数，交给PMT反演线程来执行
 * @param index 线程索引
 * @param N_Bri 布里渊光子数
 * @param N_Rayleigh 瑞利光子数
 * @param SNR 信噪比
 * @param receivedDataContainer 存储接收数据的容器
 * @param depth 深度
 */
void PageDataGenerator::receiveSystemCallbackFunc(int index, double N_Bri, double N_Rayleigh, double SNR, QVector<QVector<double> *> *receivedDataContainer, double depth)
{
    // receivedDataContainer->replace(index, new QVector<double>({N_Bri, N_Rayleigh, SNR}));
    // qDebug() << "receivedDataContainer->size():" << receivedDataContainer->size();
    QVector<double> *underwaterData = UnderWaterSpectrumDataGenerator::getUnderWaterSpectrumDataByNAndSNR(N_Bri, N_Rayleigh, SNR);
    qDebug() << "underwaterData:" << underwaterData->at(1100);
    QVector<double> *afterFizeauData = FizeauIFGenerator::getAfterFizeauSpectrumData(underwaterData);
    qDebug() << "afterFizeauData:" << afterFizeauData->at(1100);
    QVector<double> *afterPMTData = PMTReceptionDataGenerator::receiveSpectrumAfterPMT(afterFizeauData);
    QVector<double> *retrievalData = PMTReceptionDataGenerator::retrievalBySpecializePMT(afterPMTData, depth);
    receivedDataContainer->replace(index, retrievalData);

    qDebug() << "receivedDataContainer index:" << index;
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

/**
 * @brief 将页面的信息传递给悬浮窗口
 * @param index 页面索引
 * @param info 信息
 */
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

/**
 * @brief 多线程任务完成后的槽函数，统一在此处理
 * @param args 参数列表
 * @param taskName 任务名称
 */
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
            Singleton<Logger>::getInstance()->logMessage("Screenshot failed!", Logger::Warning);
        }
        else if (args->at(0).toInt() == -1)
        {
            Singleton<Logger>::getInstance()->logMessage("Unable to retrieve screen!",
                                                         Logger::Warning);
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

/**
 * @brief 存储运行时数据
 * @param dataContainer 数据容器
 * @param page_index 页面索引
 * @param curve_index 曲线索引
 * @param page_type 页面类型，用来分辨主要的绘图界面和扩展的动态绘图界面
 */
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
        Singleton<Logger>::getInstance()->logMessage("No file path selected!", Logger::Warning);
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
        Singleton<Logger>::getInstance()->logMessage("No file path selected!", Logger::Warning);
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
        Singleton<Logger>::getInstance()->logMessage("No file path selected!", Logger::Warning);
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
        Singleton<Logger>::getInstance()->logMessage("No file path selected!", Logger::Warning);
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
        Singleton<Logger>::getInstance()->logMessage("No file path selected!", Logger::Warning);
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
        Singleton<Logger>::getInstance()->logMessage("No file path selected!", Logger::Warning);
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
        Singleton<Logger>::getInstance()->logMessage("No file path selected!", Logger::Warning);
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
        Singleton<Logger>::getInstance()->logMessage("No file path selected!", Logger::Warning);
        return;
    }

    // 提示选择了多少个文件
    // Singleton<Logger>::getInstance()->logMessage("已选择" + QString::number(filePaths.size()) + "个文件！", Logger::Info);
    Singleton<Logger>::getInstance()->logMessage(QString::number(filePaths.size())
                                                     + "files have been selected!",
                                                 Logger::Info);
    // 给xDataVector和yDataVector内部的QVector<QVector<double> *>分配filePaths.size()的内存
    for (int i = 0; i < filePaths.size(); ++i)
    {
        xDataVector->append(new QVector<QVector<double> *>);
        yDataVector->append(new QVector<QVector<double> *>);
    }

    Singleton<ConstantStorage>::getInstance(nullptr)->importAllPageRuntimeDataFromCSVFile(filePaths, xDataVector, yDataVector);
    emit importConstantCompleted(0, xDataVector, yDataVector);
}
