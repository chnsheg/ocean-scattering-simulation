#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QWidget>
#include "buttongroupmanager.h"
#include "inputdatalistmanager.h"
#include "ui_mainwindow.h"

/***************************视图层***************************/

class ViewController : public QWidget
{
    Q_OBJECT
private:
    static ViewController *viewControllerInstance; // 全局唯一的ViewController实例
    Ui::MainWindow *ui;
    ViewController(Ui::MainWindow *_ui, QWidget *parent = nullptr);

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

signals:
    void startButtonClicked(const InputDataListManager &inputDataList);

public slots:
    void updateViewCurveSlot(const QVector<double> *xData, const QVector<double> *yData, int count);

private slots:
    void startButtonClicked();
};

#endif // VIEWCONTROLLER_H
