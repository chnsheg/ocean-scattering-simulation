#include "manager/customplotmanager.h"
#include "manager/tracermanager.h"

/**为什么此处不继承MangerBase类？
 * 实际上是可以继承ManagerBase类的，但是此处不继承的原因是：
 * 1.CustomPlotManager的同类事件比较少；
 * 2.大部分产生的事件在内部已经完成处理，不需要再向外部传递；
 * 3.基本没有涉及业务逻辑，只是对QCustomPlot进行了一些简单的封装；
 */

CustomPlotManager *CustomPlotManager::customPlotManagerInstance = nullptr;
/**
 * @brief CustomPlotManager::CustomPlotManager
 * @param _customPlot
 * 构造函数
 */

CustomPlotManager::CustomPlotManager(QCustomPlot *_customPlot, QObject *parent)
    : QObject(parent)
    , customPlot(_customPlot)
{
    // 初始化 QCustomPlot 样式
    initCustomPlotStyle();
    //挂载轨迹管理器
    TracerManager::getTracerManagerInstance(_customPlot);
    TracerManager::getTracerManagerInstance()->setTracerVisible(false);
    stopGenerateTracerEventSignal();
    // connect(customPlot, &QCustomPlot::mouseMove, this, &CustomPlotManager::handleMouseMove);
    // void (CustomPlotManager::*mouseMoveSignal)(QMouseEvent *event, QVector<QColor> colorVector)
    //     = &CustomPlotManager::mouseMoveSignal;
    // void (TracerManager::*showTracer)(QMouseEvent *event, QVector<QColor> colorVector)
    //     = &TracerManager::showTracer;
    // connect(this, mouseMoveSignal, TracerManager::getTracerManagerInstance(), showTracer);
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
    TracerManager::getTracerManagerInstance()->setTracerVisible(false);
    stopGenerateTracerEventSignal();
    customPlot = newCustomPlot;
    initCustomPlotStyle();
    TracerManager::getTracerManagerInstance()->setTracerCustomPlot(newCustomPlot);
}

/**
 * @brief CustomPlotManager::getCustomPlot
 * 获取 QCustomPlot 实例
 */
QCustomPlot *CustomPlotManager::getCustomPlot()
{
    return customPlot;
}

void CustomPlotManager::plotGraphToBuffer(const QVector<double> *xData,
                                          const QVector<double> *yData,
                                          int curve_index)
{
    // 绘制曲线
    customPlot->addGraph();
    auto maxElement = std::min_element(yData->begin(), yData->end(), [](double a, double b) {
        return (a >= b); //此处决定了应该将最大值更大的曲线放在最后再绘制
    });
    //对横坐标范围进行限定，下界向下取整，上界向上取整
    customPlot->xAxis->setRange(floor(*xData->begin()), ceil(*xData->end()));
    customPlot->yAxis->setRange(0, *maxElement);
    customPlot->graph(curve_index)->setPen(QPen(colorContainer.at(curve_index), 3)); //设置曲线颜色
    customPlot->graph(curve_index)->setData(*xData, *yData, true); //必须确保数据是有序的
    customPlot->legend->setVisible(true);
    customPlot->graph(curve_index)->rescaleAxes(true);
    customPlot->replot(); //重绘 每次改变完以后都要调用这个进行重新绘制
}

void CustomPlotManager::plotGraph(const QVector<double> *xData,
                                  const QVector<double> *yData,
                                  int curve_index)
{
    plotGraphToBuffer(xData, yData, curve_index);
    refreshPlot();
}

void CustomPlotManager::setLegendName(const QString &name, int curve_index)
{
    customPlot->graph(curve_index)->setName(name);
}

void CustomPlotManager::refreshPlot()
{
    customPlot->replot();
}

void CustomPlotManager::clearPlot()
{
    customPlot->clearGraphs();
    customPlot->replot();
}

void CustomPlotManager::hidePlot()
{
    stopGenerateTracerEventSignal();
    //隐藏曲线显示
    for (int i = 0; i < customPlot->graphCount(); i++) {
        customPlot->graph(i)->setVisible(false);
    }
    //隐藏图例显示
    customPlot->legend->setVisible(false);
    //隐藏跟踪点
    TracerManager::getTracerManagerInstance()->setTracerVisible(false);
    stopGenerateTracerEventSignal();
    customPlot->replot();
}

void CustomPlotManager::showPlot()
{
    //显示曲线显示
    for (int i = 0; i < customPlot->graphCount(); i++) {
        customPlot->graph(i)->setVisible(true);
    }
    //显示图例显示
    customPlot->legend->setVisible(true);
    //显示跟踪点
    TracerManager::getTracerManagerInstance()->setTracerVisible(false);
    stopGenerateTracerEventSignal();
    customPlot->replot();
}

int CustomPlotManager::getCount()
{
    return customPlot->graphCount();
}

bool CustomPlotManager::getTracerStatus()
{
    return TracerManager::getTracerManagerInstance()->getTracerVisible();
}

/**
 * @brief CustomPlotManager::changeTracerStatus
 * @return
 * 改变跟踪点的状态
 */
bool CustomPlotManager::changeTracerStatus()
{
    bool visible = getTracerStatus();
    TracerManager::getTracerManagerInstance()->setTracerVisible(!visible);
    if (visible) {
        stopGenerateTracerEventSignal();
        refreshPlot();
    } else {
        startGenerateTracerEventSignal();
    }
    return !visible;
}

void CustomPlotManager::stopGenerateTracerEventSignal()
{
    disconnect(customPlot, &QCustomPlot::mouseMove, this, &CustomPlotManager::handleMouseMove);
    void (CustomPlotManager::*mouseMoveSignal)(QMouseEvent *event, QVector<QColor> colorVector)
        = &CustomPlotManager::mouseMoveSignal;
    void (TracerManager::*showTracer)(QMouseEvent *event, QVector<QColor> colorVector)
        = &TracerManager::showTracer;
    disconnect(this, mouseMoveSignal, TracerManager::getTracerManagerInstance(), showTracer);
}

void CustomPlotManager::startGenerateTracerEventSignal()
{
    connect(customPlot, &QCustomPlot::mouseMove, this, &CustomPlotManager::handleMouseMove);
    void (CustomPlotManager::*mouseMoveSignal)(QMouseEvent *event, QVector<QColor> colorVector)
        = &CustomPlotManager::mouseMoveSignal;
    void (TracerManager::*showTracer)(QMouseEvent *event, QVector<QColor> colorVector)
        = &TracerManager::showTracer;
    connect(this, mouseMoveSignal, TracerManager::getTracerManagerInstance(), showTracer);
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
        //等比例移动第二条坐标轴
        void (QCPAxis::*rangeChanged)(const QCPRange &, const QCPRange &) = &QCPAxis::rangeChanged;
        connect(customPlot->yAxis,
                rangeChanged,
                customPlot->yAxis2,
                [=](const QCPRange &newRange, const QCPRange &oldRange) {
                    QCPRange y2Range = customPlot->yAxis2->range();
                    double dy = newRange.lower - oldRange.lower;
                    double rate = dy / (oldRange.upper - oldRange.lower);
                    double y2Lower = y2Range.lower + rate * (y2Range.upper - y2Range.lower);
                    double y2Upper = y2Range.upper + rate * (y2Range.upper - y2Range.lower);
                    customPlot->yAxis2->setRange(y2Lower, y2Upper);
                });
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
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    customPlot->graph(index)->rescaleAxes(true);
}

void CustomPlotManager::handleMouseMove(QMouseEvent *event)
{
    emit mouseMoveSignal(event, colorContainer);
}