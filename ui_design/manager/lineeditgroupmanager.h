#ifndef LINEEDITGROUPMANAGER_H
#define LINEEDITGROUPMANAGER_H

#include <QObject>
#include <QVector>
#include "base/singleton.h"
#include <QLineEdit>
// #include "manager/managerbase.h"     // 如果需要在lineEdit内容修改时就存储到常量中，就需要继承ManagerBase

class LineEditGroup
{
public:
    LineEditGroup(QList<QLineEdit *> lineEdits, int index);
    QMap<QLineEdit *, QString> getLineEdits() const
    {
        return m_lineEdits;
    }
    void saveLineEditGroupText();

private:
    QMap<QLineEdit *, QString> m_lineEdits;
};

class LineEditGroupManager : public QObject
{
    SINGLETON(LineEditGroupManager)
    Q_OBJECT
public:
    void saveLineEditGroupsText(int index);

private:
    LineEditGroupManager(QList<QList<QLineEdit *>> lineEditsList, QObject *parent = nullptr);

    QVector<LineEditGroup> m_lineEditGroups;
signals:
};

#endif // LINEEDITGROUPMANAGER_H
