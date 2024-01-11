#ifndef PAGEPLOT_H
#define PAGEPLOT_H

/***************************控制层***************************/

#include <QObject>
#include "buttongroup.h"
#include "mytracer.h"
#include "qcustomplot.h"

class PagePlot : public QObject
{
    Q_OBJECT
public:
    static PagePlot *getPagePlotInstance(int _page_index);
    static PagePlot *getPagePlotInstance();
    static void destroyPagePlotInstance();

    void initializePlot();         //初始化绘图区
    void loadTracer();             //加载跟踪点
    void plotGraph(int count = 0); //绘制曲线
    void freeData();               //释放数据
    void switchPage();             //仅用在从主界面进入绘图界面的时候触发

public slots:
    void startPlot();
    void clearPlot();
    void showTracer(QMouseEvent *event);
    void changeTracerStatus();

private:
    explicit PagePlot(int _page_index, QObject *parent = nullptr);
    ~PagePlot();
    static PagePlot *pagePlotInstance;

    int page_index;
    QCustomPlot *customPlot;
    QVector<double> *xData;
    QVector<double> *yData;

    QSharedPointer<myTracer> m_TraserX;
    QSharedPointer<myTracer> m_TracerY;

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

    ButtonStatus ButtonWaitForOpen = {true, false, false}; // 等待开启显示的按钮状态结构体
    ButtonStatus ButtonWaitForClose = {false, true, true}; // 等待关闭显示的按钮状态结构体
};

#endif // PAGEPLOT_H
