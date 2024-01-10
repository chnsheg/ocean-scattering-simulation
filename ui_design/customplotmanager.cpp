#include "customplotmanager.h"

CustomPlotManager *CustomPlotManager::customPlotManagerInstance = nullptr;

/**
 * @brief CustomPlotManager::CustomPlotManager
 * @param _customPlot
 * 构造函数
 */
CustomPlotManager::CustomPlotManager(QCustomPlot *_customPlot)
    : customPlot(_customPlot)
{
    // 初始化 QCustomPlot 样式
    initCustomPlotStyle();
}

/**
 * @brief CustomPlotManager::getInstance
 * @param customPlot
 * @return
 * 获取 QCustomPlotManager 实例
 */
CustomPlotManager *CustomPlotManager::getCustomPlotManagerInstance(QCustomPlot *customPlot)
{
    if (!customPlotManagerInstance) {
        customPlotManagerInstance = new CustomPlotManager(customPlot);
    }
    return customPlotManagerInstance;
}

/**
 * @brief CustomPlotManager::getInstance
 * @return
 * 获取 QCustomPlotManager 实例
 */
CustomPlotManager *CustomPlotManager::getCustomPlotManagerInstance()
{
    if (!customPlotManagerInstance) {
        // 在这里你可能想要抛出一个异常或者采取其他处理方式
        // 因为没有指定 QCustomPlot 的实例，单例模式无法正常工作
        return nullptr;
    }
    return customPlotManagerInstance;
}

/**
 * @brief CustomPlotManager::initCustomPlotStyle
 * 初始化 QCustomPlot 样式
 */
void CustomPlotManager::initCustomPlotStyle()
{
    // 进行 QCustomPlot 样式初始化，你可以根据需要添加代码
    customPlot->xAxis->setLabel("x");                         //设置x轴名称
    customPlot->yAxis->setLabel("y");                         //设置y轴名称
    customPlot->xAxis->setLabelColor(QColor(226, 60, 255));   //设置x轴名称颜色
    customPlot->yAxis->setLabelColor(QColor(226, 60, 255));   //设置y轴名称颜色
    customPlot->xAxis->setTickLabelColor(Qt::yellow);         //设置x轴坐标颜色
    customPlot->yAxis->setTickLabelColor(Qt::yellow);         //设置y轴坐标颜色
    customPlot->xAxis->setBasePen(QPen(QColor(0, 0, 0)));     //设置x轴坐标轴颜色
    customPlot->yAxis->setBasePen(QPen(QColor(25, 150, 92))); //设置y轴坐标轴颜色
    //设置画布背景色
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);

    // 显示图例
    customPlot->legend->setVisible(true);
    // 设置图例的位置
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    //设置legend的字体大小
    customPlot->legend->setFont(QFont("Consolas", 12));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes
                                | QCP::iSelectLegend | QCP::iSelectPlottables);
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    customPlot->axisRect()->setRangeZoomAxes(customPlot->xAxis, nullptr);
    customPlot->axisRect()->setRangeZoomFactor(0.8);
    customPlot->replot(); //重绘 每次改变完以后都要调用这个进行重新绘制
}

/**
 * @brief CustomPlotManager::setCustomPlot
 * @param newCustomPlot
 * 设置 QCustomPlot 实例
 */
void CustomPlotManager::setCustomPlot(QCustomPlot *newCustomPlot)
{
    customPlot = newCustomPlot;
}

/**
 * @brief CustomPlotManager::getCustomPlot
 * 获取 QCustomPlot 实例
 */
QCustomPlot *CustomPlotManager::getCustomPlot()
{
    return customPlot;
}

/**
 * @brief CustomPlotManager::createSecondAxis
 * @param lower 第二个坐标轴的下限
 * @param upper 第二个坐标轴的上限
 * @param label 第二个坐标轴的标签
 * 创建第二个坐标轴
 */
void CustomPlotManager::createSecondAxis(double lower, double upper, QString label)
{
    // 创建第二个坐标轴
    if (customPlot->yAxis2 != nullptr) {
        customPlot->yAxis2->setVisible(true);
        customPlot->yAxis2->setTickLabels(true);
        customPlot->yAxis2->setRange(lower, upper);
        customPlot->yAxis2->setTickLabelColor(Qt::yellow);
        customPlot->yAxis2->setLabelColor(QColor(226, 60, 255));
        //设置第二条坐标轴可以拖动
        customPlot->yAxis2->setLabel(label);
    }
}

/**
 * @brief CustomPlotManager::switchToSecondAxis
 * @param index 曲线索引,即第几条曲线使用第二个坐标轴
 * 切换到第二个坐标轴
 */
void CustomPlotManager::switchToSecondAxis(int index)
{
    // 切换到第二个坐标轴
    customPlot->graph(index)->setValueAxis(customPlot->yAxis2);
}
