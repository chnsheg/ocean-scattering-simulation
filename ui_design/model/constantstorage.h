#ifndef CONSTANTSTORAGE_H
#define CONSTANTSTORAGE_H

#include <QObject>
#include <QMap>
#include <QJsonValue>
#include "base/singleton.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include "model/constantmap.h"
#include "module/qcustomplot.h"

Q_DECLARE_METATYPE(QSharedPointer<QCPGraphDataContainer>)

class ConstantStorage : public QObject
{
    SINGLETON(ConstantStorage);
    Q_OBJECT
public:
    explicit ConstantStorage(QObject *parent = nullptr);

    // 设置常量的方法
    void setConstant(const QString &name, const QVariant &value);

    // 获取常量的方法
    QVariant getConstant(const QString &name) const;

    // 获取所有常量的 JSON 格式表示
    QJsonObject getAllConstantsJson() const;

private:
    // 常量的存储容器
    QMap<QString, QVariant> m_constants;
    void saveToJsonFile(const QString &fileName) const;
    bool loadFromJsonFile(const QString &fileName);

signals:
};

#endif // CONSTANTSTORAGE_H
