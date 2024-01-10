#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QDebug>
#include <QPushButton>
#include <QVector>

struct ButtonStatus // 控制按钮组状态结构体 按键组：控制显示状态的一组按键
{
    bool showEnabled;
    bool clearEnabled;
    bool tracerEnabled;
};

// 按键状态的结构体
class ButtonGroup
{
public:
    QPushButton *showButton;
    QPushButton *clearButton;
    QPushButton *tracerButton;
    QPushButton *backButton;
    // 可以添加更多按钮指针
public:
    // 默认构造函数
    ButtonGroup()
        : showButton(nullptr)
        , clearButton(nullptr)
        , tracerButton(nullptr)
        , backButton(nullptr)
    {}
    ButtonGroup(QPushButton *show, QPushButton *clear, QPushButton *tracer, QPushButton *back)
        : showButton(show)
        , clearButton(clear)
        , tracerButton(tracer)
        , backButton(back)
    {}
};

class ButtonGroups
{
private:
    // 单例模式构造
    static ButtonGroups *ButtonGroupsInstance;
    ButtonGroups(QVector<ButtonGroup> *_buttonGroups);

    // 存储每个页面的按钮状态
    QVector<ButtonGroup> *buttonGroups;

public:
    // 单例模式获取实例
    static ButtonGroups *getButtonGroupsInstance(QVector<ButtonGroup> *buttonGroups);
    static ButtonGroups *getButtonGroupsInstance();

    // 释放实例
    static void destroyButtonGroupsInstance();

    // 更新按钮状态
    void updateButtonStatus(int index, const ButtonStatus &status);
    void updateTracerButtonText(int index, bool isVisible);
};

#endif // BUTTONGROUP_H
