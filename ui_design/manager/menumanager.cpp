#include "menumanager.h"

MenuManager::MenuManager(QMenuBar *menuBar, QObject *parent) : ManagerBase(parent), menuBar(menuBar)
{
    addEvent("menu1_menu1_action1",
             "menu1_menu1_action1",
             menuBar->actions().at(0)->menu()->actions().at(0)->menu()->actions().at(0),
             &QAction::triggered,
             MenuActionId::Menu1_Menu1_Action1);
    addEvent("menu1_menu1_action2",
             "menu1_menu1_action2",
             menuBar->actions().at(0)->menu()->actions().at(0)->menu()->actions().at(2),
             &QAction::triggered,
             MenuActionId::Menu1_Menu1_Action2);
    addEvent("menu1_menu2_action1",
             "menu1_menu2_action1",
             menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(0),
             &QAction::triggered,
             MenuActionId::Menu1_Menu2_Action1);
    addEvent("menu1_menu2_action2",
             "menu1_menu2_action2",
             menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(2),
             &QAction::triggered,
             MenuActionId::Menu1_Menu2_Action2);
    addEvent("menu2_menu1_action1",
             "menu2_menu1_action1",
             menuBar->actions().at(1)->menu()->actions().at(0)->menu()->actions().at(0),
             &QAction::triggered,
             MenuActionId::Menu2_Menu1_Action1);
    addEvent("menu2_menu1_action2",
             "menu2_menu1_action2",
             menuBar->actions().at(1)->menu()->actions().at(0)->menu()->actions().at(2),
             &QAction::triggered,
             MenuActionId::Menu2_Menu1_Action2);
    addEvent("menu2_menu2_action1",
             "menu2_menu2_action1",
             menuBar->actions().at(1)->menu()->actions().at(1)->menu()->actions().at(0),
             &QAction::triggered,
             MenuActionId::Menu2_Menu2_Action1);
    addEvent("menu2_menu2_action2",
             "menu2_menu2_action2",
             menuBar->actions().at(1)->menu()->actions().at(1)->menu()->actions().at(2),
             &QAction::triggered,
             MenuActionId::Menu2_Menu2_Action2);
}

MenuManager::MenuManager() {}

MenuManager::~MenuManager()
{
    // 释放内存
    if (menuBar != nullptr)
    {
        delete menuBar;
        menuBar = nullptr;
    }
}

void MenuManager::onEventAction(const QString &event, int status, const QVariant &param)
{
    MenuActionId menuActionId = param.value<MenuActionId>();
    emit eventSignal(menuActionId);
}
