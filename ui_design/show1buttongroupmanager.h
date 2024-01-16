#ifndef SHOW1BUTTONGROUPMANAGER_H
#define SHOW1BUTTONGROUPMANAGER_H

#include <QObject>
#include <QPushButton>
#include "managerbase.h"

class Show1ButtonGroup
{
public:
    QPushButton *showButton_1;
    QPushButton *showButton_2;
    QPushButton *showButton_3;
    QPushButton *showButton_4;
    QPushButton *showButton_5;

public:
    Show1ButtonGroup(QPushButton *show_1,
                     QPushButton *show_2,
                     QPushButton *show_3,
                     QPushButton *show_4,
                     QPushButton *show_5)
        : showButton_1(show_1)
        , showButton_2(show_2)
        , showButton_3(show_3)
        , showButton_4(show_4)
        , showButton_5(show_5)
    {}
    Show1ButtonGroup()
        : showButton_1(nullptr)
        , showButton_2(nullptr)
        , showButton_3(nullptr)
        , showButton_4(nullptr)
        , showButton_5(nullptr)
    {}
};

enum ButtonId { ShowButton_1, ShowButton_2, ShowButton_3, ShowButton_4, ShowButton_5 };
Q_DECLARE_METATYPE(ButtonId)
class Show1ButtonGroupManager : public ManagerBase
{
    Q_OBJECT

private:
    static Show1ButtonGroupManager *Show1ButtonGroupManagerInstance;
    Show1ButtonGroupManager(Show1ButtonGroup *_show1ButtonGroup, QObject *parent = nullptr);
    Show1ButtonGroup *show1ButtonGroup;

public:
    static Show1ButtonGroupManager *getShow1ButtonGroupManagerInstance(
        Show1ButtonGroup *_show1ButtonGroup);
    static Show1ButtonGroupManager *getShow1ButtonGroupManagerInstance();
    static void destroyShow1ButtonGroupManagerInstance();

    //保留接口
    void initShow1ButtonGroupStyle();
    void initShow1ButtonGroupStatus();

    Show1ButtonGroup *getShow1ButtonGroup();
    void onEventAction(const QString &event, int status, const QVariant &param);

public:
signals:
    // void show1ButtonGroupClicked(int index);
    void eventSignal(int param);
    // private slots:
    //     void show1ButtonClicked();
    //     void show2ButtonClicked();
    //     void show3ButtonClicked();
    //     void show4ButtonClicked();
    //     void show5ButtonClicked();
};

#endif // SHOW1BUTTONGROUPMANAGER_H
