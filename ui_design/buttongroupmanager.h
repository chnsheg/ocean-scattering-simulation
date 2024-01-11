#ifndef BUTTONGROUPMANAGER_H
#define BUTTONGROUPMANAGER_H

#include <QDebug>
#include <QPushButton>
#include <QVector>

struct ButtonStatus // 控制按钮组状态结构体 按键组：控制显示状态的一组按键
{
    bool showEnabled;
    bool clearEnabled;
    bool tracerEnabled;

    ButtonStatus(bool show, bool clear, bool tracer)
        : showEnabled(show)
        , clearEnabled(clear)
        , tracerEnabled(tracer)
    {}
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

class ButtonGroupsManager
{
private:
    // 单例模式构造
    static ButtonGroupsManager *ButtonGroupsManagerInstance;
    ButtonGroupsManager(QVector<ButtonGroup> *_buttonGroups);

    // 存储每个页面的按钮状态
    QVector<ButtonGroup> *buttonGroups;

public:
    // 单例模式获取实例
    static ButtonGroupsManager *getButtonGroupsManagerInstance(QVector<ButtonGroup> *buttonGroups);
    static ButtonGroupsManager *getButtonGroupsManagerInstance();

    // 释放实例
    static void destroyButtonGroupsManagerInstance();

    //初始化按钮样式
    void initButtonStyle(int index);
    //初始化按钮状态
    void initButtonStatus(int index);

    // 更新按钮状态
    void updateButtonStatus(int index, const ButtonStatus &status);
    void updateTracerButtonText(int index, bool isVisible);
};

#endif // BUTTONGROUPMANAGER_H
