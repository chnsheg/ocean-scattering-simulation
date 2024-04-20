#include "manager/show1buttongroupmanager.h"
#include <QDebug>

Show1ButtonGroupManager::Show1ButtonGroupManager(Show1ButtonGroup *_show1ButtonGroup,
                                                 QObject *parent)
    : ManagerBase(parent), show1ButtonGroup(_show1ButtonGroup)
{
    show1ButtonGroup = _show1ButtonGroup;

    addEvent("show1ButtonClicked",
             "show1ButtonClicked",
             show1ButtonGroup->showButton_1,
             &HoverButton::clicked,
             Show1ButtonGroupId::ShowButton_1);
    addEvent("show2ButtonClicked",
             "show2ButtonClicked",
             show1ButtonGroup->showButton_2,
             &HoverButton::clicked,
             Show1ButtonGroupId::ShowButton_2);
    addEvent("show3ButtonClicked",
             "show3ButtonClicked",
             show1ButtonGroup->showButton_3,
             &HoverButton::clicked,
             Show1ButtonGroupId::ShowButton_3);
    addEvent("show4ButtonClicked",
             "show4ButtonClicked",
             show1ButtonGroup->showButton_4,
             &HoverButton::clicked,
             Show1ButtonGroupId::ShowButton_4);
    addEvent("show5ButtonClicked",
             "show5ButtonClicked",
             show1ButtonGroup->showButton_5,
             &HoverButton::clicked,
             Show1ButtonGroupId::ShowButton_5);
    addEvent("show6ButtonClicked",
             "show6ButtonClicked",
             show1ButtonGroup->showButton_6,
             &HoverButton::clicked,
             Show1ButtonGroupId::ShowButton_6);
    addEvent("show7ButtonClicked",
             "show7ButtonClicked",
             show1ButtonGroup->showButton_7,
             &HoverButton::clicked,
             Show1ButtonGroupId::ShowButton_7);
    addEvent("show8ButtonClicked",
             "show8ButtonClicked",
             show1ButtonGroup->showButton_8,
             &HoverButton::clicked,
             Show1ButtonGroupId::ShowButton_8);

    addEvent("show1ButtonHovered",
             "show1ButtonHovered",
             show1ButtonGroup->showButton_1,
             &HoverButton::hoverSignal,
             Show1ButtonGroupId::HoverButton_1);

    addEvent("show2ButtonHovered",
             "show2ButtonHovered",
             show1ButtonGroup->showButton_2,
             &HoverButton::hoverSignal,
             Show1ButtonGroupId::HoverButton_2);

    addEvent("show3ButtonHovered",
             "show3ButtonHovered",
             show1ButtonGroup->showButton_3,
             &HoverButton::hoverSignal,
             Show1ButtonGroupId::HoverButton_3);

    addEvent("show4ButtonHovered",
             "show4ButtonHovered",
             show1ButtonGroup->showButton_4,
             &HoverButton::hoverSignal,
             Show1ButtonGroupId::HoverButton_4);

    addEvent("show5ButtonHovered",
             "show5ButtonHovered",
             show1ButtonGroup->showButton_5,
             &HoverButton::hoverSignal,
             Show1ButtonGroupId::HoverButton_5);

    addEvent("show6ButtonHovered",
             "show6ButtonHovered",
             show1ButtonGroup->showButton_6,
             &HoverButton::hoverSignal,
             Show1ButtonGroupId::HoverButton_6);

    addEvent("show7ButtonHovered",
             "show7ButtonHovered",
             show1ButtonGroup->showButton_7,
             &HoverButton::hoverSignal,
             Show1ButtonGroupId::HoverButton_7);

    addEvent("show8ButtonHovered",
             "show8ButtonHovered",
             show1ButtonGroup->showButton_8,
             &HoverButton::hoverSignal,
             Show1ButtonGroupId::HoverButton_8);

    addEvent("show1ButtonReleased",
             "show1ButtonReleased",
             show1ButtonGroup->showButton_1,
             &HoverButton::leaveSignal,
             Show1ButtonGroupId::leaveButton_1);

    addEvent("show2ButtonReleased",
             "show2ButtonReleased",
             show1ButtonGroup->showButton_2,
             &HoverButton::leaveSignal,
             Show1ButtonGroupId::leaveButton_2);

    addEvent("show3ButtonReleased",
             "show3ButtonReleased",
             show1ButtonGroup->showButton_3,
             &HoverButton::leaveSignal,
             Show1ButtonGroupId::leaveButton_3);

    addEvent("show4ButtonReleased",
             "show4ButtonReleased",
             show1ButtonGroup->showButton_4,
             &HoverButton::leaveSignal,
             Show1ButtonGroupId::leaveButton_4);

    addEvent("show5ButtonReleased",
             "show5ButtonReleased",
             show1ButtonGroup->showButton_5,
             &HoverButton::leaveSignal,
             Show1ButtonGroupId::leaveButton_5);

    addEvent("show6ButtonReleased",
             "show6ButtonReleased",
             show1ButtonGroup->showButton_6,
             &HoverButton::leaveSignal,
             Show1ButtonGroupId::leaveButton_6);

    addEvent("show7ButtonReleased",
             "show7ButtonReleased",
             show1ButtonGroup->showButton_7,
             &HoverButton::leaveSignal,
             Show1ButtonGroupId::leaveButton_7);

    addEvent("show8ButtonReleased",
             "show8ButtonReleased",
             show1ButtonGroup->showButton_8,
             &HoverButton::leaveSignal,
             Show1ButtonGroupId::leaveButton_8);
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
    // 先判断param是否为Show1ButtonGroupId类型
    if (param.canConvert<Show1ButtonGroupId>())
    {
        Show1ButtonGroupId buttonId = param.value<Show1ButtonGroupId>();
        if (buttonId < Show1ButtonGroupId::HoverButton_1)
        {
            emit eventSignal(buttonId);
        }
        else if (buttonId >= Show1ButtonGroupId::HoverButton_1 && buttonId <= Show1ButtonGroupId::HoverButton_8)
        {
            switch (buttonId)
            {
            case Show1ButtonGroupId::HoverButton_1:
                emit hoverSignal(0, show1ButtonGroup->showButton_1->pos());
                break;
            case Show1ButtonGroupId::HoverButton_2:
                emit hoverSignal(1, show1ButtonGroup->showButton_2->pos());
                break;
            case Show1ButtonGroupId::HoverButton_3:
                emit hoverSignal(2, show1ButtonGroup->showButton_3->pos());
                break;
            case Show1ButtonGroupId::HoverButton_4:
                emit hoverSignal(3, show1ButtonGroup->showButton_4->pos());
                break;
            case Show1ButtonGroupId::HoverButton_5:
                emit hoverSignal(4, show1ButtonGroup->showButton_5->pos());
                break;
            case Show1ButtonGroupId::HoverButton_6:
                emit hoverSignal(5, show1ButtonGroup->showButton_6->pos());
                break;
            case Show1ButtonGroupId::HoverButton_7:
                emit hoverSignal(6, show1ButtonGroup->showButton_7->pos());
                break;
            case Show1ButtonGroupId::HoverButton_8:
                emit hoverSignal(7, show1ButtonGroup->showButton_8->pos());
                break;
            }
        }
        else if (buttonId >= Show1ButtonGroupId::leaveButton_1 && buttonId <= Show1ButtonGroupId::leaveButton_8)
        {
            emit leaveSignal(buttonId);
        }
    }
}