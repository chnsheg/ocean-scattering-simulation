#ifndef BUTTONGROUPMANAGER_H
#define BUTTONGROUPMANAGER_H

#include <QDebug>
#include <QObject>
#include <QPushButton>
#include <QVector>
#include "base/singleton.h"
#include "manager/managerbase.h"
#include <QHBoxLayout>

struct ButtonStatus // 控制按钮组状态结构体 按键组：控制显示状态的一组按键
{
    bool showEnabled;
    bool clearEnabled;
    bool tracerEnabled;

    ButtonStatus(bool show, bool clear, bool tracer)
        : showEnabled(show), clearEnabled(clear), tracerEnabled(tracer)
    {
    }
};

// 按键状态的结构体,按功能定义
class ButtonGroup
{
public:
    QPushButton *showButton;
    QPushButton *clearButton;
    QPushButton *tracerButton;
    QPushButton *backButton;
    QPushButton *saveConstantButton;
    // 可以添加更多按钮指针
public:
    // 默认构造函数
    ButtonGroup()
        : showButton(nullptr), clearButton(nullptr), tracerButton(nullptr), backButton(nullptr), saveConstantButton(nullptr)
    {
    }
    ButtonGroup(QPushButton *show, QPushButton *clear, QPushButton *tracer, QPushButton *back, QPushButton *saveConstant)
        : showButton(show), clearButton(clear), tracerButton(tracer), backButton(back), saveConstantButton(saveConstant)
    {
    }
};

enum ButtonGroupId
{
    showButton,
    clearButton,
    tracerButton,
    back1Button,
    back2Button,
    back3Button,
    back4Button,
    back5Button,
    back6Button,
    back7Button,
    back8Button,
    dynamic1Button,
    dynamic2Button,
    dynamic3Button,
};
Q_DECLARE_METATYPE(ButtonGroupId)

class ButtonGroupsManager : public ManagerBase
{
    Q_OBJECT
    SINGLETON(ButtonGroupsManager);

private:
    // 单例模式构造
    // static ButtonGroupsManager *ButtonGroupsManagerInstance;
    ButtonGroupsManager(QVector<ButtonGroup> *_buttonGroups, QObject *parent = nullptr);

    // 存储每个页面的按钮状态
    QVector<ButtonGroup> *buttonGroups;
    QMap<QPushButton *, QString> dynamicButtons; // 用于存储动态创建的按钮

public:
    // // 单例模式获取实例
    // static ButtonGroupsManager *getButtonGroupsManagerInstance(QVector<ButtonGroup> *buttonGroups);
    // static ButtonGroupsManager *getButtonGroupsManagerInstance();

    // // 释放实例
    // static void destroyButtonGroupsManagerInstance();

    // 初始化按钮样式
    void initButtonStyle(int index);
    // 初始化按钮状态
    void initButtonStatus(int index);

    // 更新按钮状态
    void updateButtonStatus(int index, const ButtonStatus &status);
    void updateTracerButtonText(int index, bool isVisible);

    // 添加新的按钮
    void addDynamicButton(QWidget *parent, const QString &buttonName);

    // 获取buttonGroups
    //  QVector<ButtonGroup> *getButtonGroups();

    void onEventAction(const QString &event, int status, const QVariant &param);

public:
signals:
    void eventSignal(ButtonGroupId param); // 用来区分返回按键索引的信号
};

#endif // BUTTONGROUPMANAGER_H
