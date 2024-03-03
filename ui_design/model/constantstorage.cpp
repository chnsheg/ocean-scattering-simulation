#include "constantstorage.h"
#include <qDebug>
#include "module/qcustomplot.h"
#include "utils/logger.h"
#include "utils/readfiledata.h"

ConstantStorage::ConstantStorage(QObject *parent)
    : QObject{parent}
{
    bool debug = true;
    if ((!loadFromJsonFile("constants.json")) || debug == true)
    {
        // 默认值
        ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
        m_constants.insert(constantMap->getConstantName(0, 0), 1e8);
        m_constants.insert(constantMap->getConstantName(0, 1), 5.32e-07);
        m_constants.insert(constantMap->getConstantName(0, 2), 1);
        m_constants.insert(constantMap->getConstantName(0, 3), 12000000000);
        m_constants.insert(constantMap->getConstantName(0, 4), 299792458.0);
        m_constants.insert(constantMap->getConstantName(1, 0), 20);
        m_constants.insert(constantMap->getConstantName(1, 1), 35);
        // m_constants.insert(constantMap->getConstantName(1, 2), 1.0);
        // m_constants.insert(constantMap->getConstantName(1, 3), 12e9);

        // m_constants.insert(constantMap->getConstantName(2, 0), 532e-9);
        // m_constants.insert(constantMap->getConstantName(2, 1), 1.0);
        // m_constants.insert(constantMap->getConstantName(2, 2), 10e-12);
        // m_constants.insert(constantMap->getConstantName(2, 3), 10e-6);
        // m_constants.insert(constantMap->getConstantName(3, 0), 0.25);
        // m_constants.insert(constantMap->getConstantName(3, 1), 100.0);
        // m_constants.insert(constantMap->getConstantName(3, 2), 1.0e6);
        // m_constants.insert(constantMap->getConstantName(3, 3), 1.0e-9);
        // m_constants.insert(constantMap->getConstantName(4, 0), 532e-9);
        // m_constants.insert(constantMap->getConstantName(4, 1), 1.0);
        // m_constants.insert(constantMap->getConstantName(4, 2), 10e-12);
        // m_constants.insert(constantMap->getConstantName(4, 3), 10e-6);
        // for (int i = 0; i < 8; i++)
        // {
        //     m_constants.insert(constantMap->getConstantName(5, i), QVariant::fromValue(QSharedPointer<QCPGraphDataContainer>::create()));
        // }
        saveToJsonFile(CONSTANT_FILE);
    }
}

ConstantStorage::~ConstantStorage()
{
    saveToJsonFile(CONSTANT_FILE);
}

void ConstantStorage::setConstant(const QString &name, const QVariant &value)
{
    m_constants[name] = value;
}

QVariant ConstantStorage::getConstant(const QString &name) const
{
    qDebug() << "name: " << name << "value: " << m_constants[name];
    return m_constants[name];
}

QJsonObject ConstantStorage::getAllConstantsJson() const
{
    QJsonObject jsonObject;
    for (const auto &name : m_constants.keys())
    {
        jsonObject[name] = QJsonValue::fromVariant(m_constants[name]);
    }
    return QJsonObject(jsonObject);
}

void ConstantStorage::saveToJsonFile(const QString &fileName) const
{
    QFile file{fileName};
    // 打印保存的路径
    qDebug() << "save path: " << QDir::currentPath();
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Cannot open file for writing:" << fileName;
        return;
    }
    file.write(QJsonDocument{getAllConstantsJson()}.toJson());
    file.close();
}

bool ConstantStorage::loadFromJsonFile(const QString &fileName)
{
    QFile file{fileName};
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Cannot open file for reading:" << fileName;
        return false;
    }
    const auto jsonDocument = QJsonDocument::fromJson(file.readAll());
    file.close();
    const auto jsonObject = jsonDocument.object();
    for (const auto &name : jsonObject.keys())
    {
        m_constants[name] = jsonObject[name].toVariant();
        qDebug() << "jsonName: " << name << "jsonValue: " << m_constants[name];
    }
    return true;
}

void ConstantStorage::savePageConstantToJsonFile(int index, const QStringList &filePaths)
{
    // 可以搞个save(all?)复选框来判断
    // 1、先根据index获取当前页面的常数名称，再根据名称去QMap<QString, QVariant> m_constants获取当前页面的常数值
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    int i = 0;
    QMap<QString, QVariant> temp;
    while (true)
    {
        QString name = constantMap->getConstantName(index, i);
        if (name == QString())
        {
            break;
        }
        temp[name] = m_constants[name];
        i++;
    }

    // 保存当前页面的常数到"constants.json"；注意不是清空文件后再写入，而是在原有的基础上更改
    // QFile file{CONSTANT_FILE};
    QFile file{filePaths.at(0)};
    if (!file.exists())
    {
        Singleton<Logger>::getInstance()->logMessage("File " + QDir::currentPath() + "/" + CONSTANT_FILE + " does not exist", Logger::Warning);
        Singleton<Logger>::getInstance()->logMessage("Try to create new files");
        if (file.open(QIODevice::WriteOnly))
        {
            file.close();
            Singleton<Logger>::getInstance()->logMessage("Create new files " + QDir::currentPath() + "/" + CONSTANT_FILE, Logger::Info);
        }
        else
        {
            Singleton<Logger>::getInstance()->logMessage("Create new files failed", Logger::Warning);
            return;
        }
    }
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Cannot open file for reading:" << filePaths.at(0);
        return;
    }
    auto jsonDocument = QJsonDocument::fromJson(file.readAll());
    file.close();
    auto jsonObject = jsonDocument.object();
    for (const auto &name : temp.keys())
    {
        // 更新或追加键值对
        jsonObject[name] = QJsonValue::fromVariant(temp[name]);
    }
    if (!file.open(QIODevice::WriteOnly))
    {
        // qWarning() << "Cannot open file for writing:" << CONSTANT_FILE;
        Singleton<Logger>::getInstance()->logMessage("Cannot open file for writing", Logger::Warning);
        return;
    }
    file.write(QJsonDocument{jsonObject}.toJson());
    file.close();

    // 使用Logger打印保存路径
    Singleton<Logger>::getInstance()->logMessage("Save current page's constants to " + QDir::currentPath() + "/" + filePaths.at(0), Logger::Info);
}

void ConstantStorage::saveAllPageConstantToJsonFile(const QStringList &filePaths)
{
    // saveToJsonFile(CONSTANT_FILE);
    saveToJsonFile(filePaths.at(0));
    Singleton<Logger>::getInstance()->logMessage("Save all pages' constants to " + QDir::currentPath() + "/" + filePaths.at(0), Logger::Info);
}

void ConstantStorage::convertQSharedPointerToQVector(QSharedPointer<QCPGraphDataContainer> dataContainer, QVector<double> *xDataVector, QVector<double> *yDataVector)
{
    QCPDataContainer<QCPGraphData> *dataContainerData = dataContainer.data();
    int size = dataContainerData->size();
    xDataVector->resize(size);
    yDataVector->resize(size);

    for (int i = 0; i < size; i++)
    {
        // xDataVector->push_back(dataContainerData->at(i)->mainKey());
        // yDataVector->push_back(dataContainerData->at(i)->mainValue());
        xDataVector->replace(i, dataContainerData->at(i)->mainKey());
        yDataVector->replace(i, dataContainerData->at(i)->mainValue());
    }
}

void ConstantStorage::savePageRuntimeDataToCSVFile(int index, const QStringList &filePaths)
{
    QVector<double> *xDataVector, *yDataVector;
    QString filePath;
    // 1、先根据index获取当前页面的常数名称，再根据名称去QMap<QString, QVariant> m_constants获取当前页面的常数值
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    QSharedPointer<QCPGraphDataContainer> dataContainer;

    switch (index)
    {
    case 0:
        xDataVector = new QVector<double>();
        yDataVector = new QVector<double>();
        dataContainer = m_constants[constantMap->getConstantName(5, 0)].value<QSharedPointer<QCPGraphDataContainer>>();
        if (dataContainer.isNull())
        {
            Singleton<Logger>::getInstance()->logMessage("No runtime data of page1", Logger::Warning);
            return;
        }
        convertQSharedPointerToQVector(dataContainer, xDataVector, yDataVector);
        ReadFileData::saveDataToCSVFile(xDataVector, yDataVector, filePaths.at(0));
        delete xDataVector;
        delete yDataVector;
        xDataVector = nullptr;
        yDataVector = nullptr;
        dataContainer.clear();
        dataContainer = nullptr;
        Singleton<Logger>::getInstance()->logMessage("Save runtime data of page1 to " + QDir::currentPath() + "/runtime_data_page1.csv", Logger::Info);
        break;
    case 1:
        for (int i = 0; i < 3; i++)
        {
            xDataVector = new QVector<double>();
            yDataVector = new QVector<double>();
            dataContainer = m_constants[constantMap->getConstantName(5, i + 1)].value<QSharedPointer<QCPGraphDataContainer>>();
            if (dataContainer.isNull())
            {
                Singleton<Logger>::getInstance()->logMessage("No runtime data of page2_" + QString::number(i), Logger::Warning);
                continue;
            }
            convertQSharedPointerToQVector(dataContainer, xDataVector, yDataVector);
            // ReadFileData::saveDataToCSVFile(xDataVector, yDataVector, "runtime_data_page2_" + QString::number(i) + ".csv");
            filePath = filePaths.at(i);
            if (filePath.isEmpty())
            {
                Singleton<Logger>::getInstance()->logMessage("No file path of page2_" + QString::number(i), Logger::Warning);
            }
            else
            {
                ReadFileData::saveDataToCSVFile(xDataVector, yDataVector, filePath);
            }
            delete xDataVector;
            delete yDataVector;
            xDataVector = nullptr;
            yDataVector = nullptr;
            dataContainer.clear();
            dataContainer = nullptr;
            Singleton<Logger>::getInstance()->logMessage("Save runtime data of page2 to " + filePath, Logger::Info);
        }
        break;
    case 2:
        xDataVector = new QVector<double>();
        yDataVector = new QVector<double>();
        dataContainer = m_constants[constantMap->getConstantName(5, 4)].value<QSharedPointer<QCPGraphDataContainer>>();
        if (dataContainer.isNull())
        {
            Singleton<Logger>::getInstance()->logMessage("No runtime data of page3", Logger::Warning);
            return;
        }
        convertQSharedPointerToQVector(dataContainer, xDataVector, yDataVector);
        // ReadFileData::saveDataToCSVFile(xDataVector, yDataVector, "runtime_data_page3.csv");
        filePath = filePaths.at(0);
        if (filePath.isEmpty())
        {
            Singleton<Logger>::getInstance()->logMessage("No file path of page3", Logger::Warning);
        }
        else
        {
            ReadFileData::saveDataToCSVFile(xDataVector, yDataVector, filePath);
        }
        delete xDataVector;
        delete yDataVector;
        xDataVector = nullptr;
        yDataVector = nullptr;
        dataContainer.clear();
        dataContainer = nullptr;
        Singleton<Logger>::getInstance()->logMessage("Save runtime data of page3 to " + filePath, Logger::Info);
        break;
    case 3:
        for (int i = 0; i < 2; i++)
        {
            xDataVector = new QVector<double>();
            yDataVector = new QVector<double>();
            dataContainer = m_constants[constantMap->getConstantName(5, i + 5)].value<QSharedPointer<QCPGraphDataContainer>>();
            if (dataContainer.isNull())
            {
                Singleton<Logger>::getInstance()->logMessage("No runtime data of page4_" + QString::number(i), Logger::Warning);
                continue;
            }
            convertQSharedPointerToQVector(dataContainer, xDataVector, yDataVector);
            // ReadFileData::saveDataToCSVFile(xDataVector, yDataVector, "runtime_data_page4_" + QString::number(i) + ".csv");
            filePath = filePaths.at(i);
            if (filePath.isEmpty())
            {
                Singleton<Logger>::getInstance()->logMessage("No file path of page4_" + QString::number(i), Logger::Warning);
            }
            else
            {
                ReadFileData::saveDataToCSVFile(xDataVector, yDataVector, filePath);
            }
            delete xDataVector;
            delete yDataVector;
            xDataVector = nullptr;
            yDataVector = nullptr;
            dataContainer.clear();
            dataContainer = nullptr;
            Singleton<Logger>::getInstance()->logMessage("Save runtime data of page4 to " + filePath + QString::number(i) + ".csv", Logger::Info);
        }
        break;
    case 4:
        xDataVector = new QVector<double>();
        yDataVector = new QVector<double>();
        dataContainer = m_constants[constantMap->getConstantName(5, 7)].value<QSharedPointer<QCPGraphDataContainer>>();
        if (dataContainer.isNull())
        {
            Singleton<Logger>::getInstance()->logMessage("No runtime data of page5", Logger::Warning);
            return;
        }
        convertQSharedPointerToQVector(dataContainer, xDataVector, yDataVector);
        // ReadFileData::saveDataToCSVFile(xDataVector, yDataVector, "runtime_data_page5.csv");
        filePath = filePaths.at(0);
        if (filePath.isEmpty())
        {
            Singleton<Logger>::getInstance()->logMessage("No file path of page5", Logger::Warning);
        }
        else
        {
            ReadFileData::saveDataToCSVFile(xDataVector, yDataVector, filePath);
        }
        delete xDataVector;
        delete yDataVector;
        xDataVector = nullptr;
        yDataVector = nullptr;
        dataContainer.clear();
        dataContainer = nullptr;
        Singleton<Logger>::getInstance()->logMessage("Save runtime data of page5 to " + filePath, Logger::Info);
        break;
    default:
        break;
    }
}

void ConstantStorage::saveAllPageRuntimeDataToCSVFile(const QStringList &filePaths)
{
    for (int i = 0; i < 5; i++)
    {
        savePageRuntimeDataToCSVFile(i, filePaths);
    }
    Singleton<Logger>::getInstance()->logMessage("Save all pages' runtime data to " + QDir::currentPath(), Logger::Info);
}