#ifndef DYNAMICVIEW_H
#define DYNAMICVIEW_H

#include <QPushButton>
#include <QStackedWidget>
#include <QVector>
#include <QWidget>
#include "module/qcustomplot.h"

class DynamicPage : public QWidget
{
    Q_OBJECT

public:
    QVector<QColor> colorContainer = {QColor(226, 60, 255),
                                      QColor(64, 224, 208),
                                      QColor(255, 99, 71),
                                      QColor(255, 255, 0),
                                      QColor(0, 255, 255),
                                      QColor(255, 0, 0),
                                      QColor(0, 255, 0),
                                      QColor(0, 0, 255),
                                      QColor(255, 255, 255),
                                      QColor(0, 0, 0)};
    // 构造函数需要页面总数和可选的父窗口指针
    explicit DynamicPage(int pageCount, QWidget *parent = nullptr);
    // 析构函数
    ~DynamicPage();

    // 显示指定页面的曲线，需要页码、X数据集、Y数据集和图表标题
    void displayCurve(int pageIndex,
                      QVector<QVector<double> *> *xData,
                      QVector<QVector<double> *> *yData,
                      QString title,
                      QStringList legendList = QStringList());

    void initCustomPlotStyle(QCustomPlot *customPlot);

    int getCurveNum(int pageIndex);

private slots:
    // 处理“上一页面”按钮点击事件
    void onPrevPageClicked();
    // 处理“下一页面”按钮点击事件
    void onNextPageClicked();
    // 处理“显示光标”按钮点击事件
    void onShowCursorClicked();
    // 处理“关闭”按钮点击事件
    void onCloseClicked();

public slots:
    void updateDynamicView(QVector<QVector<double> *> *xData,
                           QVector<QVector<double> *> *yData,
                           int index,
                           QString title,
                           QStringList legendList = QStringList());

signals:
    void storeRuntimeDataSignal(QSharedPointer<QCPGraphDataContainer> dataContainer, const int page_index, const int curve_index); // 用于存储运行时数据
    void closeDynamicPageSignal(int index);                                                                                        // 用于关闭动态页面
private:
    // 存储页面切换组件
    QStackedWidget *stackedWidget;
    // 存储每个页面的QCustomPlot对象
    QVector<QCustomPlot *> *plots;
    // 控制页面切换的按钮
    QPushButton *prevPageButton;
    QPushButton *nextPageButton;
    // 显示光标和关闭页面的按钮
    QPushButton *showCursorButton;
    QPushButton *closeButton;
    // 页面总数
    int pageCount;

    static int dynamicPageObjectNum; // 用于追踪DynamicPage对象的数量

    int pageObjectId;

    // 初始化界面元素
    void setupUi();
    // 创建一个页面，包括图表和必要的按钮
    void createPage(int index);
};
#endif // DYNAMICVIEW_H
