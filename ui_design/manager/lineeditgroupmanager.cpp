#include "lineeditgroupmanager.h"
#include "model/constantmap.h"
#include "model/constantstorage.h"
#include <qDebug>

/**
 * 该类用于绑定所有lineEdit到特定的常量名称，方便在程序中根据常量名称获取lineEdit的值。
 */

LineEditGroupManager::LineEditGroupManager(QList<QList<QLineEdit *>> lineEditsList, QList<QList<QLabel *>> labelsList, QObject *parent) : QObject(parent)
{
    // int i = 0;
    // for (auto lineEdits : lineEditsList)
    // {
    //     m_lineEditGroups.append(LineEditGroup{
    //         lineEdits, i++});
    // }
    for (int i = 0; i < lineEditsList.size(); i++)
    {
        m_lineEditGroups.append(LineEditGroup{
            lineEditsList[i], labelsList[i], i});
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
    // m_lineEditGroups[index].saveLineEditGroupText();
    // 保存所有页面的输入
    for (auto lineEditGroup : m_lineEditGroups)
    {
        lineEditGroup.saveLineEditGroupText();
    }
}

LineEditGroup::LineEditGroup(QList<QLineEdit *> lineEdits, QList<QLabel *> labels, int index)
{
    // int i = 0;
    // for (auto lineEdit : lineEdits)
    // {
    //     switch (index)
    //     {
    //     case 0:
    //         m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(0, i++));
    //         break;
    //     case 1:
    //         m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(1, i++));
    //         break;
    //     case 2:
    //         m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(7, i++));
    //         break;
    //     case 3:
    //         m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(2, i++));
    //         break;
    //     case 4:
    //         m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(3, i++));
    //         break;
    //     case 5:
    //         m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(7, i++));
    //         break;
    //     case 6:
    //         m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(7, i++));
    //         break;
    //     case 7:
    //         m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(7, i++));
    //         break;
    //     case 8:
    //         m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(7, i++));
    //         break;
    //     default:
    //         m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(index, i++));
    //         break;
    //     }
    //     // m_lineEdits.insert(lineEdit, Singleton<ConstantMap>::getInstance()->getConstantName(index, i++));
    //     lineEdit->setText(Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(m_lineEdits[lineEdit]).toString());
    // }

    for (int i = 0; i < lineEdits.size(); i++)
    {
        switch (index)
        {
        case 0:
            m_lineEdits.insert(lineEdits[i], Singleton<ConstantMap>::getInstance()->getConstantName(0, i));
            m_labels.insert(labels[i], Singleton<ConstantMap>::getInstance()->getConstantName(0, i));
            break;
        case 1:
            m_lineEdits.insert(lineEdits[i], Singleton<ConstantMap>::getInstance()->getConstantName(1, i));
            m_labels.insert(labels[i], Singleton<ConstantMap>::getInstance()->getConstantName(1, i));
            break;
        case 2:
            m_lineEdits.insert(lineEdits[i], Singleton<ConstantMap>::getInstance()->getConstantName(7, i));
            m_labels.insert(labels[i], Singleton<ConstantMap>::getInstance()->getConstantName(7, i));
            break;
        case 3:
            m_lineEdits.insert(lineEdits[i], Singleton<ConstantMap>::getInstance()->getConstantName(2, i));
            m_labels.insert(labels[i], Singleton<ConstantMap>::getInstance()->getConstantName(2, i));
            break;
        case 4:
            m_lineEdits.insert(lineEdits[i], Singleton<ConstantMap>::getInstance()->getConstantName(3, i));
            m_labels.insert(labels[i], Singleton<ConstantMap>::getInstance()->getConstantName(3, i));
            break;
            // case 5:
            //     m_lineEdits.insert(lineEdits[i], Singleton<ConstantMap>::getInstance()->getConstantName(7, i));
            //     m_labels.insert(labels[i], Singleton<ConstantMap>::getInstance()->getConstantName(7, i));
            //     break;
            // case 6:
            //     m_lineEdits.insert(lineEdits[i], Singleton<ConstantMap>::getInstance()->getConstantName(7, i));
            //     m_labels.insert(labels[i], Singleton<ConstantMap>::getInstance()->getConstantName(7, i));
            //     break;
            // case 7:
            //     m_lineEdits.insert(lineEdits[i], Singleton<ConstantMap>::getInstance()->getConstantName(7, i));
            //     m_labels.insert(labels[i], Singleton<ConstantMap>::getInstance()->getConstantName(7, i));
            //     break;
            // case 8:
            //     m_lineEdits.insert(lineEdits[i], Singleton<ConstantMap>::getInstance()->getConstantName(7, i));
            //     m_labels.insert(labels[i], Singleton<ConstantMap>::getInstance()->getConstantName(7, i));
            //     break;
            // default:
            //     m_lineEdits.insert(lineEdits[i], Singleton<ConstantMap>::getInstance()->getConstantName(index, i));
            //     m_labels.insert(labels[i], Singleton<ConstantMap>::getInstance()->getConstantName(index, i));
            //     break;
        }
        lineEdits[i]->setText(Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(m_lineEdits[lineEdits[i]]).toString());
        // labels[i]->setText(m_labels[labels[i]]);
        QString labelText = m_labels[labels[i]];
        if (labelText != "")
        {
            labels[i]->setText(labelText);
        }
        else
        {
            // 隐藏label和对应的lineEdit
            labels[i]->hide();
            lineEdits[i]->hide();
        }
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
