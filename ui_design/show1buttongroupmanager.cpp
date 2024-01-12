#include "show1buttongroupmanager.h"

Show1ButtonGroupManager *Show1ButtonGroupManager::Show1ButtonGroupManagerInstance = nullptr;

Show1ButtonGroupManager::Show1ButtonGroupManager(Show1ButtonGroup *_show1ButtonGroup,
                                                 QObject *parent)
    : QObject(parent)
    , show1ButtonGroup(_show1ButtonGroup)
{
    show1ButtonGroup = _show1ButtonGroup;
    connect(show1ButtonGroup->showButton_1,
            &QPushButton::clicked,
            this,
            &Show1ButtonGroupManager::show1ButtonClicked);
    connect(show1ButtonGroup->showButton_2,
            &QPushButton::clicked,
            this,
            &Show1ButtonGroupManager::show2ButtonClicked);
    connect(show1ButtonGroup->showButton_3,
            &QPushButton::clicked,
            this,
            &Show1ButtonGroupManager::show3ButtonClicked);
    connect(show1ButtonGroup->showButton_4,
            &QPushButton::clicked,
            this,
            &Show1ButtonGroupManager::show4ButtonClicked);
    connect(show1ButtonGroup->showButton_5,
            &QPushButton::clicked,
            this,
            &Show1ButtonGroupManager::show5ButtonClicked);
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

void Show1ButtonGroupManager::show1ButtonClicked()
{
    emit show1ButtonGroupClicked(1);
}

void Show1ButtonGroupManager::show2ButtonClicked()
{
    emit show1ButtonGroupClicked(2);
}

void Show1ButtonGroupManager::show3ButtonClicked()
{
    emit show1ButtonGroupClicked(3);
}

void Show1ButtonGroupManager::show4ButtonClicked()
{
    emit show1ButtonGroupClicked(4);
}

void Show1ButtonGroupManager::show5ButtonClicked()
{
    emit show1ButtonGroupClicked(5);
}
