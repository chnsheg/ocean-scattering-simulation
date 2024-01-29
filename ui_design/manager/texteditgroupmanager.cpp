#include "texteditgroupmanager.h"
#include "model/constantmap.h"
#include <qDebug>

/**
 * 该类用于绑定所有lineEdit到特定的常量名称，方便在程序中根据常量名称获取lineEdit的值。
 */

LineEditGroupManager::LineEditGroupManager(QList<QList<QLineEdit *>> lineEditsList, QObject *parent)
    : QObject{parent}
{
    int i = 0;
    for (auto lineEdits : lineEditsList)
    {
        m_lineEditGroups.append(LineEditGroup{
            lineEdits, i++});
    }
}

LineEditGroupManager::LineEditGroupManager()
{
    // This is a private constructor, so it can't be called.
}

LineEditGroupManager::~LineEditGroupManager()
{
    // This is a private destructor, so it can't be called.
}

LineEditGroup::LineEditGroup(QList<QLineEdit *> lineEdits, int index)
{
    int i = 0;
    for (auto lineEdit : lineEdits)
    {
        m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(index, i++));
        lineEdit->setText(m_lineEdits[lineEdit]);
    }
}
