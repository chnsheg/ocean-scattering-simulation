#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QWidget>
#include "manager/buttongroupmanager.h"
#include "manager/inputdatalistmanager.h"
#include "manager/show1buttongroupmanager.h"
#include "ui_mainwindow.h"

/***************************视图层***************************/

class ViewController : public QWidget
{
    Q_OBJECT
private:
    static ViewController *viewControllerInstance; // 全局唯一的ViewController实例
    Ui::MainWindow *ui;
    ViewController(Ui::MainWindow *_ui, QWidget *parent = nullptr);

private:
    const QList<int> plotPageIndex = {1, 2, 3, 4, 5};
    const QList<int> showPageIndex = {0}; // 用于区分展示界面和绘图界面

public:
    // 单例模式
    static ViewController *getViewControllerInstance(Ui::MainWindow *_ui);
    static ViewController *getViewControllerInstance();
    static void destroyViewControllerInstance();

    // 管理视图层属性和样式
    int getCurrentPageIndex();
    QCustomPlot *getCurrentPageCustomPlot();

    // // 集中管理按钮状态
    // void updateButtonStatus(const ButtonStatus &status);
    // void updateTracerButtonText(bool isVisible);

signals:
    void onStartButtonClicked(const InputDataListManager *inputDataList);
    void onClearButtonClicked();
    void onTracerButtonClicked();
    void switchPageButtonClicked(int page_index); //interface_index 用于区分返回的界面

public slots:
    void updateViewCurveSlot(const QVector<double> *xData,
                             const QVector<double> *yData,
                             int curve_index);
    void updateViewStyleSlot(int plotInterfaceIndex);
    void updateViewTracerSlot();
    void updateViewClearSlot();
    void updateViewPageSlot(int page_index);

private:
    void handleButtonGroupManagerEvent(ButtonGroupId buttonGroupId);
    void handleShow1ButtonGroupManagerEvent(Show1ButtonGroupId buttonGroupId);
    void startButtonClicked();
    void clearButtonClicked();
    void tracerButtonClicked();
    void switchPlotPageButtonClicked(int index);
    void switchShowPageButtonClicked(int index);
};

#endif // VIEWCONTROLLER_H
