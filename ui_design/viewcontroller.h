#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include "buttongroupmanager.h"
#include "ui_mainwindow.h"

/***************************视图层***************************/

class ViewController
{
private:
    static ViewController *viewControllerInstance; // 全局唯一的ViewController实例
    Ui::MainWindow *ui;
    ViewController(Ui::MainWindow *_ui);

public:
    // 单例模式
    static ViewController *getViewControllerInstance(Ui::MainWindow *_ui);
    static ViewController *getViewControllerInstance();
    static void destroyViewControllerInstance();

    // 管理视图层属性和样式
    int getCurrentPageIndex();
    QCustomPlot *getCurrentPageCustomPlot();
    void updateViewStyle();

    // 集中管理按钮状态
    void updateButtonStatus(const ButtonStatus &status);
    void updateTracerButtonText(bool isVisible);
};

#endif // VIEWCONTROLLER_H
