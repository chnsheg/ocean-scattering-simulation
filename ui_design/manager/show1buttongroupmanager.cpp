#include "manager/show1buttongroupmanager.h"

Show1ButtonGroupManager *Show1ButtonGroupManager::Show1ButtonGroupManagerInstance = nullptr;

Show1ButtonGroupManager::Show1ButtonGroupManager(Show1ButtonGroup *_show1ButtonGroup,
                                                 QObject *parent)
    : ManagerBase(parent)
    , show1ButtonGroup(_show1ButtonGroup)
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
}

Show1ButtonGroupManager *Show1ButtonGroupManager::getShow1ButtonGroupManagerInstance(
    Show1ButtonGroup *_show1ButtonGroup)
{
    if (!Show1ButtonGroupManagerInstance) {
        Show1ButtonGroupManagerInstance = new Show1ButtonGroupManager(_show1ButtonGroup);
    }
    return Show1ButtonGroupManagerInstance;
}

Show1ButtonGroupManager *Show1ButtonGroupManager::getShow1ButtonGroupManagerInstance()
{
    if (!Show1ButtonGroupManagerInstance) {
        // 在这里你可能想要抛出一个异常或者采取其他处理方式
        // 因为没有指定 QCustomPlot 的实例，单例模式无法正常工作
        return nullptr;
    }
    return Show1ButtonGroupManagerInstance;
}

void Show1ButtonGroupManager::destroyShow1ButtonGroupManagerInstance()
{
    if (Show1ButtonGroupManagerInstance) {
        delete Show1ButtonGroupManagerInstance;
        Show1ButtonGroupManagerInstance = nullptr;
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
