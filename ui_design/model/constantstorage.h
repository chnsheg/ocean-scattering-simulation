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
#include <QVector>

Q_DECLARE_METATYPE(QSharedPointer<QCPGraphDataContainer>)
Q_DECLARE_METATYPE(QVector<double> *)
Q_DECLARE_METATYPE(QPixmap *)

class ConstantStorage : public QObject
{
#define CONSTANT_FILE "constants.json"
    SINGLETON(ConstantStorage);
    Q_OBJECT
public:
    // explicit ConstantStorage(QObject *parent = nullptr);

    // 设置常量的方法
    void setConstant(const QString &name, const QVariant &value);

    // 获取常量的方法
    QVariant getConstant(const QString &name) const;

    // 获取所有常量的 JSON 格式表示
    QJsonObject getAllConstantsJson() const;

    // 保存常量到文件
    void savePageConstantToJsonFile(int index, const QStringList &filePaths);
    void saveAllPageConstantToJsonFile(const QStringList &filePaths);
    void savePageRuntimeDataToCSVFile(int index, const QStringList &filePaths);
    void saveAllPageRuntimeDataToCSVFile(const QStringList &filePaths);

    // 从文件中导入常量
    void importPageConstantFromJsonFile(const QStringList &filePaths);
    void importAllPageConstantFromJsonFile(const QStringList &filePaths);
    void importPageRuntimeDataFromCSVFile(int index, const QStringList &filePaths, QVector<QVector<double> *> *xDataVector,
                                          QVector<QVector<double> *> *yDataVector);
    void importAllPageRuntimeDataFromCSVFile(const QStringList &filePaths, QVector<QVector<QVector<double> *> *> *xDataVector,
                                             QVector<QVector<QVector<double> *> *> *yDataVector);

    // 将QSharedPointer<QCPGraphDataContainer>变量转换为QVector<double>变量，方便存储到csv文件中
    void convertQSharedPointerToQVector(QSharedPointer<QCPGraphDataContainer> dataContainer,
                                        QVector<double> *xDataVector,
                                        QVector<double> *yDataVector);

private:
    explicit ConstantStorage(QObject *parent = nullptr);
    void saveToJsonFile(const QString &fileName) const;
    bool loadFromJsonFile(const QString &fileName);

    // 常量的存储容器
    QMap<QString, QVariant> m_constants;

signals:
};

#endif // CONSTANTSTORAGE_H
