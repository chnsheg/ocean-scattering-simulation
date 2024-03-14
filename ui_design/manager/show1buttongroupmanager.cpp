#include "manager/show1buttongroupmanager.h"

Show1ButtonGroupManager::Show1ButtonGroupManager(Show1ButtonGroup *_show1ButtonGroup,
                                                 QObject *parent)
    : ManagerBase(parent), show1ButtonGroup(_show1ButtonGroup)
{
    show1ButtonGroup = _show1ButtonGroup;
    addEvent("show1ButtonClicked",
             "show1ButtonClicked",
             show1ButtonGroup->showButton_1,
             &QPushButton::clicked,
             Show1ButtonGroupId::ShowButton_1);
    addEvent("show2ButtonClicked",
             "show2ButtonClicked",
             show1ButtonGroup->showButton_2,
             &QPushButton::clicked,
             Show1ButtonGroupId::ShowButton_2);
    addEvent("show3ButtonClicked",
             "show3ButtonClicked",
             show1ButtonGroup->showButton_3,
             &QPushButton::clicked,
             Show1ButtonGroupId::ShowButton_3);
    addEvent("show4ButtonClicked",
             "show4ButtonClicked",
             show1ButtonGroup->showButton_4,
             &QPushButton::clicked,
             Show1ButtonGroupId::ShowButton_4);
    addEvent("show5ButtonClicked",
             "show5ButtonClicked",
             show1ButtonGroup->showButton_5,
             &QPushButton::clicked,
             Show1ButtonGroupId::ShowButton_5);
    addEvent("show6ButtonClicked",
             "show6ButtonClicked",
             show1ButtonGroup->showButton_6,
             &QPushButton::clicked,
             Show1ButtonGroupId::ShowButton_6);
    addEvent("show7ButtonClicked",
             "show7ButtonClicked",
             show1ButtonGroup->showButton_7,
             &QPushButton::clicked,
             Show1ButtonGroupId::ShowButton_7);
    addEvent("show8ButtonClicked",
             "show8ButtonClicked",
             show1ButtonGroup->showButton_8,
             &QPushButton::clicked,
             Show1ButtonGroupId::ShowButton_8);
}

Show1ButtonGroupManager::Show1ButtonGroupManager() {}

Show1ButtonGroupManager::~Show1ButtonGroupManager()
{
    // 释放内存
    if (show1ButtonGroup != nullptr)
    {
        delete show1ButtonGroup;
        show1ButtonGroup = nullptr;
    }
}

void Show1ButtonGroupManager::initShow1ButtonGroupStyle()
{
    // 进行 QCustomPlot 样式初始化，你可以根据需要添加代码
}

void Show1ButtonGroupManager::initShow1ButtonGroupStatus()
{
    // 进行 QCustomPlot 状态初始化，你可以根据需要添加代码
}

Show1ButtonGroup *Show1ButtonGroupManager::getShow1ButtonGroup()
{
    return show1ButtonGroup;
}

void Show1ButtonGroupManager::onEventAction(const QString &event, int status, const QVariant &param)
{
    Show1ButtonGroupId buttonId = param.value<Show1ButtonGroupId>();
    emit eventSignal(buttonId);
}
