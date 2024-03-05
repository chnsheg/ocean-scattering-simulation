#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <QObject>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include "manager/managerbase.h"
#include "base/singleton.h"

enum MenuActionId
{
    Menu1_Menu1_Action1,
    Menu1_Menu1_Action2,
    Menu1_Menu2_Action1,
    Menu1_Menu2_Action2,
    Menu2_Menu1_Action1,
    Menu2_Menu1_Action2,
    Menu2_Menu2_Action1,
    Menu2_Menu2_Action2
};
Q_DECLARE_METATYPE(MenuActionId)

class MenuManager : public ManagerBase
{
    Q_OBJECT
    SINGLETON(MenuManager)

private:
    QMenuBar *menuBar;

    MenuManager(QMenuBar *menuBar, QObject *parent = nullptr);

    void onEventAction(const QString &event, int status, const QVariant &param);

public:
    void showPageMenuStatus();
    void plotPageMenuStatus(bool existCurve);
signals:
    void eventSignal(MenuActionId param);

signals:
};

#endif // MENUMANAGER_H
