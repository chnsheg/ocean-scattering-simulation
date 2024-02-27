#include "constantstorage.h"
#include <qDebug>
#include "module/qcustomplot.h"

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

        saveToJsonFile("constants.json");
    }
}

ConstantStorage::~ConstantStorage()
{
    saveToJsonFile("constants.json");
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