#ifndef CONSTANTMAP_H
#define CONSTANTMAP_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QMap>
#include "base/singleton.h"
// #include <QDebug>

class ConstantGroup
{
public:
    // 定义一些ConstantGroup的静态变量并初始化
    // static const ConstantGroup physicalConstantGroup;
    // static const ConstantGroup laserConstantGroup;
    // static const ConstantGroup fizeauConstantGroup;
    // static const ConstantGroup pmtConstantGroup;

    QString getConstantName(int constantIndex) const
    {
        if (constantIndex >= 0 && constantIndex < constants->size())
        {
            // QString constantName = constants->at(constantIndex);
            // qDebug() << "constantName: " << constantName;
            return constants->at(constantIndex);
        }
        return QString();
    }

    ConstantGroup();
    ConstantGroup(const ConstantGroup &);
    ConstantGroup(QVector<QString> *_constants);

private:
    QVector<QString> *constants;
};

class ConstantMap
{

    SINGLETON(ConstantMap);

public:
    QString getConstantName(int groupIndex, int constantIndex);

private:
    QMap<int, ConstantGroup *> *m_constantMap;
};

#endif // CONSTANTMAP_H
