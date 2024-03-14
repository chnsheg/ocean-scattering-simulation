#include "lineeditgroupmanager.h"
#include "model/constantmap.h"
#include "model/constantstorage.h"
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

void LineEditGroupManager::saveLineEditGroupsText(int index)
{
    m_lineEditGroups[index].saveLineEditGroupText();
}

LineEditGroup::LineEditGroup(QList<QLineEdit *> lineEdits, int index)
{
    int i = 0;
    for (auto lineEdit : lineEdits)
    {
        switch (index)
        {
        case 0:
            m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(0, i++));
            break;
        case 1:
            m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(1, i++));
            break;
        case 2:
            m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(4, i++));
            break;
        case 3:
            m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(7, i++));
            break;
        case 4:
            m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(2, i++));
            break;
        case 5:
            m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(3, i++));
            break;
        case 6:
            m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(7, i++));
            break;
        case 7:
            m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(7, i++));
            break;
        case 8:
            m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(7, i++));
            break;
        default:
            m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(index, i++));
            break;
        }
        // m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(index, i++));
        lineEdit->setText(Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(m_lineEdits[lineEdit]).toString());
    }
}

void LineEditGroup::saveLineEditGroupText()
{
    for (auto lineEdit : m_lineEdits.keys())
    {
        Singleton<ConstantStorage>::getInstance(nullptr)->setConstant(m_lineEdits[lineEdit], lineEdit->text());
        qDebug() << "lineEditName: " << m_lineEdits[lineEdit] << "lineEditValue: " << lineEdit->text();
    }
}
