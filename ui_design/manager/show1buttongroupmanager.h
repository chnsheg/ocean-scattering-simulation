#ifndef SHOW1BUTTONGROUPMANAGER_H
#define SHOW1BUTTONGROUPMANAGER_H

#include <QObject>
#include <QPushButton>
#include "base/singleton.h"
#include "manager/managerbase.h"
#include "module/hoverinfowidget.h"
#include <QPoint>
#include "module/hoverbutton.h"

class Show1ButtonGroup
{
public:
    // QPushButton *showButton_1;
    // QPushButton *showButton_2;
    // QPushButton *showButton_3;
    // QPushButton *showButton_4;
    // QPushButton *showButton_5;
    // QPushButton *showButton_6;
    // QPushButton *showButton_7;
    // QPushButton *showButton_8;
    HoverButton *showButton_1;
    HoverButton *showButton_2;
    HoverButton *showButton_3;
    HoverButton *showButton_4;
    HoverButton *showButton_5;
    HoverButton *showButton_6;
    HoverButton *showButton_7;
    HoverButton *showButton_8;

public:
    Show1ButtonGroup(HoverButton *_showButton_1,
                     HoverButton *_showButton_2,
                     HoverButton *_showButton_3,
                     HoverButton *_showButton_4,
                     HoverButton *_showButton_5,
                     HoverButton *_showButton_6,
                     HoverButton *_showButton_7,
                     HoverButton *_showButton_8)
        : showButton_1(_showButton_1),
          showButton_2(_showButton_2),
          showButton_3(_showButton_3),
          showButton_4(_showButton_4),
          showButton_5(_showButton_5),
          showButton_6(_showButton_6),
          showButton_7(_showButton_7),
          showButton_8(_showButton_8)
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
    ShowButton_8,
    HoverButton_1,
    HoverButton_2,
    HoverButton_3,
    HoverButton_4,
    HoverButton_5,
    HoverButton_6,
    HoverButton_7,
    HoverButton_8,
    leaveButton_1,
    leaveButton_2,
    leaveButton_3,
    leaveButton_4,
    leaveButton_5,
    leaveButton_6,
    leaveButton_7,
    leaveButton_8
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
    // bool eventFilter(QObject *obj, QEvent *event);

    Show1ButtonGroup *getShow1ButtonGroup();
    void onEventAction(const QString &event, int status, const QVariant &param);

public:
signals:
    void eventSignal(Show1ButtonGroupId param);
    void hoverSignal(int button_index, const QPoint &pos);
    void leaveSignal(int button_index);
};

#endif // SHOW1BUTTONGROUPMANAGER_H
