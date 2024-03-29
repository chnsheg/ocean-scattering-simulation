#ifndef SHOW1BUTTONGROUPMANAGER_H
#define SHOW1BUTTONGROUPMANAGER_H

#include <QObject>
#include <QPushButton>
#include "base/singleton.h"
#include "manager/managerbase.h"

class Show1ButtonGroup
{
public:
    QPushButton *showButton_1;
    QPushButton *showButton_2;
    QPushButton *showButton_3;
    QPushButton *showButton_4;
    QPushButton *showButton_5;
    QPushButton *showButton_6;
    QPushButton *showButton_7;
    QPushButton *showButton_8;

public:
    Show1ButtonGroup(QPushButton *show_1,
                     QPushButton *show_2,
                     QPushButton *show_3,
                     QPushButton *show_4,
                     QPushButton *show_5,
                     QPushButton *show_6,
                     QPushButton *show_7,
                     QPushButton *show_8)
        : showButton_1(show_1), showButton_2(show_2), showButton_3(show_3), showButton_4(show_4), showButton_5(show_5), showButton_6(show_6), showButton_7(show_7), showButton_8(show_8)
    {
    }
};

enum Show1ButtonGroupId
{
    ShowButton_1,
    ShowButton_2,
    ShowButton_3,
    ShowButton_4,
    ShowButton_5,
    ShowButton_6,
    ShowButton_7,
    ShowButton_8
};
Q_DECLARE_METATYPE(Show1ButtonGroupId)
class Show1ButtonGroupManager : public ManagerBase
{
    Q_OBJECT
    SINGLETON(Show1ButtonGroupManager)

private:
    Show1ButtonGroupManager(Show1ButtonGroup *_show1ButtonGroup, QObject *parent = nullptr);
    Show1ButtonGroup *show1ButtonGroup;

public:
    // 保留接口
    void initShow1ButtonGroupStyle();
    void initShow1ButtonGroupStatus();

    Show1ButtonGroup *getShow1ButtonGroup();
    void onEventAction(const QString &event, int status, const QVariant &param);

public:
signals:
    void eventSignal(Show1ButtonGroupId param);
};

#endif // SHOW1BUTTONGROUPMANAGER_H
