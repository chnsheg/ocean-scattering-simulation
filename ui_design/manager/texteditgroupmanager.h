#ifndef TEXTEDITGROUPMANAGER_H
#define TEXTEDITGROUPMANAGER_H

#include <QObject>
#include <QTextEdit>
#include <QVector>
#include "base/singleton.h"
#include <QLineEdit>

class LineEditGroup
{
public:
    LineEditGroup(QList<QLineEdit *> lineEdits, int index);
    QMap<QLineEdit *, QString> getLineEdits() const
    {
        return m_lineEdits;
    }

private:
    QMap<QLineEdit *, QString> m_lineEdits;
};

class LineEditGroupManager : public QObject
{
    SINGLETON(LineEditGroupManager)
    Q_OBJECT
public:
private:
    LineEditGroupManager(QList<QList<QLineEdit *>> lineEditsList, QObject *parent = nullptr);
    void renameLineEditGroups();
    QVector<LineEditGroup> m_lineEditGroups;
signals:
};

#endif // TEXTEDITGROUPMANAGER_H
