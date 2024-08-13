#include "manager/customplotmanager.h"
#include "manager/tracermanager.h"
#include <QFont> // Add this line to include the QFont class

/**为什么此处不继承MangerBase类？
 * 实际上是可以继承ManagerBase类的，但是此处不继承的原因是：
 * 1.CustomPlotManager的同类事件比较少；
 * 2.大部分产生的事件在内部已经完成处理，不需要再向外部传递；
 * 3.基本没有涉及业务逻辑，只是对QCustomPlot进行了一些简单的封装；
 */

// CustomPlotManager *CustomPlotManager::customPlotManagerInstance = nullptr;
/**
 * @brief CustomPlotManager::CustomPlotManager
 * @param _customPlot
 * 构造函数
 */

QMetaObject::Connection CustomPlotManager::connection1 = QMetaObject::Connection();
QMetaObject::Connection CustomPlotManager::connection2 = QMetaObject::Connection();
QMetaObject::Connection CustomPlotManager::connection3 = QMetaObject::Connection();

CustomPlotManager::CustomPlotManager(QCustomPlot *_customPlot, QObject *parent)
    : QObject(parent), customPlot(_customPlot)
{
    // 初始化 QCustomPlot 样式
    initCustomPlotStyle();
    // 挂载轨迹管理器
    Singleton<TracerManager>::getInstance(_customPlot);
    Singleton<TracerManager>::getInstance()->setTracerVisible(false);
    stopGenerateTracerEventSignal();
    // connect(customPlot, &QCustomPlot::mouseMove, this, &CustomPlotManager::handleMouseMove);
    // void (CustomPlotManager::*mouseMoveSignal)(QMouseEvent *event, QVector<QColor> colorVector)
    //     = &CustomPlotManager::mouseMoveSignal;
    // void (TracerManager::*showTracer)(QMouseEvent *event, QVector<QColor> colorVector)
    //     = &TracerManager::showTracer;
    // connect(this, mouseMoveSignal, TracerManager::getTracerManagerInstance(), showTracer);
}

CustomPlotManager::CustomPlotManager() {}

CustomPlotManager::~CustomPlotManager()
{
    // 释放内存
    if (customPlot != nullptr)
    {
        delete customPlot;
        customPlot = nullptr;
    }
}

/**
 * @brief CustomPlotManager::getCustomPlotRect
 * @return
 * 获取 QCustomPlot 的矩形区域
 */
QRect CustomPlotManager::getCustomPlotRect()
{
    return customPlot->viewport();
}

// /**
//  * @brief CustomPlotManager::getInstance
//  * @param customPlot
//  * @return
//  * 获取 QCustomPlotManager 实例
//  */
// CustomPlotManager *CustomPlotManager::getCustomPlotManagerInstance(QCustomPlot *customPlot)
// {
//     if (!customPlotManagerInstance) {
//         customPlotManagerInstance = new CustomPlotManager(customPlot);
//     }
//     return customPlotManagerInstance;
// }

// /**
//  * @brief CustomPlotManager::getInstance
//  * @return
//  * 获取 QCustomPlotManager 实例
//  */
// CustomPlotManager *CustomPlotManager::getCustomPlotManagerInstance()
// {
//     if (!customPlotManagerInstance) {
//         // 在这里你可能想要抛出一个异常或者采取其他处理方式
//         // 因为没有指定 QCustomPlot 的实例，单例模式无法正常工作
//         return nullptr;
//     }
//     return customPlotManagerInstance;
// }

/**
 * @brief CustomPlotManager::initCustomPlotStyle
 * 初始化 QCustomPlot 样式
 */
void CustomPlotManager::initCustomPlotStyle()
{
    // 进行 QCustomPlot 样式初始化，你可以根据需要添加代码
    customPlot->xAxis->setLabel("x");                         // 设置x轴名称
    customPlot->yAxis->setLabel("y");                         // 设置y轴名称
    customPlot->xAxis->setLabelColor(QColor(226, 60, 255));   // 设置x轴名称颜色
    customPlot->yAxis->setLabelColor(QColor(226, 60, 255));   // 设置y轴名称颜色
    customPlot->xAxis->setTickLabelColor(Qt::yellow);         // 设置x轴坐标颜色
    customPlot->yAxis->setTickLabelColor(Qt::yellow);         // 设置y轴坐标颜色
    customPlot->xAxis->setBasePen(QPen(QColor(0, 0, 0)));     // 设置x轴坐标轴颜色
    customPlot->yAxis->setBasePen(QPen(QColor(25, 150, 92))); // 设置y轴坐标轴颜色

    // 设置画布背景色
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
    // 设置legend的字体大小
    customPlot->legend->setFont(QFont("Consolas", 14));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    customPlot->axisRect()->setRangeZoomAxes(customPlot->xAxis, nullptr);
    customPlot->axisRect()->setRangeZoomFactor(0.8);
    customPlot->replot(); // 重绘 每次改变完以后都要调用这个进行重新绘制
}

/**
 * @brief CustomPlotManager::setCustomPlot
 * @param newCustomPlot
 * 设置 QCustomPlot 实例
 */
void CustomPlotManager::setCustomPlot(QCustomPlot *newCustomPlot)
{
    Singleton<TracerManager>::getInstance()->setTracerVisible(false);
    stopGenerateTracerEventSignal();
    customPlot = newCustomPlot;
    initCustomPlotStyle();
    Singleton<TracerManager>::getInstance()->setTracerCustomPlot(newCustomPlot);
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
 * @brief CustomPlotManager::plotGraphToBuffer
 * @param xData
 * @param yData
 * @param curve_index
 * 绘制曲线到缓冲区，注意没有调用replot()方法，需要在外部调用
 */
void CustomPlotManager::plotGraphToBuffer(const QVector<double> *xData,
                                          const QVector<double> *yData,
                                          int curve_index)
{
    // 绘制曲线
    customPlot->addGraph();
    // auto maxElement = std::max_element(yData->begin(), yData->end(), [](double a, double b)
    //                                    {
    //                                        return a < b; // 使用正确的比较器条件
    //                                    });
    // 对横坐标范围进行限定，下界向下取整，上界向上取整
    customPlot->graph(curve_index)->setPen(QPen(colorContainer.at(curve_index), 3)); // 设置曲线颜色
    // customPlot->graph(curve_index)->setData(*xData, *yData, true);                   // 必须确保数据是有序的
    // 补充代码：用下述方法：void QCPDataContainer< DataType >::add ( const QVector< DataType > &  data, bool  alreadySorted = false  )
    QVector<QCPGraphData> graphData(xData->size());
    for (int i = 0; i < xData->size(); ++i)
    {
        graphData[i].key = (*xData)[i];
        graphData[i].value = (*yData)[i];
    }

    // customPlot->graph(curve_index)->data()->add(graphData);
    customPlot->graph(curve_index)->data()->add(graphData, true);
    // customPlot->graph(curve_index)->setData(*xData, *yData, true);

    customPlot->legend->setVisible(true);
    // customPlot->xAxis->setRange(floor(*xData->begin()), ceil(*xData->end()));
    customPlot->xAxis->setRange(*xData->begin(), *xData->end());
    // customPlot->yAxis->setRange(0, *maxElement);
    customPlot->graph(curve_index)->rescaleAxes(); // 此处不能设置为true，会导致连续点击时坐标轴无法正常缩放
    // customPlot->replot();                              // 重绘 此处若不断重绘会导致坐标轴范围出现问题
}

/// @brief 设置画笔粗细为1，用于流畅绘制
/// @param curve_index
void CustomPlotManager::setPenToRunFluently(int curve_index)
{
    customPlot->graph(curve_index)->setPen(QPen(colorContainer.at(curve_index), 1));
}

/**
 * @brief CustomPlotManager::plotGraph
 * @param xData
 * @param yData
 * @param curve_index
 * 绘制曲线
 */
void CustomPlotManager::plotGraph(const QVector<double> *xData,
                                  const QVector<double> *yData,
                                  int curve_index)
{
    plotGraphToBuffer(xData, yData, curve_index);
    refreshPlot();
}

/**
 * @brief CustomPlotManager::plotBarGraphToBuffer
 * @param xData
 * @param yData
 * @param curve_index
 * @param legendName
 * 绘制柱状图到缓冲区，注意总共绘制两个柱状图，一个是噪声柱状图，一个是纯净柱状图，并且一个curve_index为1，一个curve_index为2
 */
void CustomPlotManager::plotBarGraphToBuffer(const QVector<double> *xData,
                                             const QVector<double> *yData,
                                             int curve_index, QString legendName)
{
    auto maxElement = std::max_element(yData->begin(), yData->end(), [](double a, double b)
                                       {
                                           return a < b; // 使用正确的比较器条件
                                       });
    // 绘制柱状图
    QCPAxis *xAxis = customPlot->xAxis2; // x轴
    QCPAxis *yAxis = customPlot->yAxis2; // y轴

    QCPBars *bars = nullptr; // 初始化bars为null指针

    // 检查是否已存在名为"Bars"的QCPBars实例
    for (int i = 0; i < customPlot->plottableCount(); ++i)
    {
        // if (qobject_cast<QCPBars *>(customPlot->plottable(i)))
        // {
        //     bars = qobject_cast<QCPBars *>(customPlot->plottable(i));
        //     // 这里假设customPlot只能有一个柱形图，如果有多个柱形图，可以在这里继续循环或者退出循环
        //     break;
        // }

        if (customPlot->plottable(i)->objectName() == "NoisedBars" && curve_index == 1)
        {
            bars = qobject_cast<QCPBars *>(customPlot->plottable(i));
            // 这里假设customPlot只能有一个柱形图，如果有多个柱形图，可以在这里继续循环或者退出循环
            break;
        }
        else if (customPlot->plottable(i)->objectName() == "PureBars" && curve_index == 2)
        {
            bars = qobject_cast<QCPBars *>(customPlot->plottable(i));
            // 这里假设customPlot只能有一个柱形图，如果有多个柱形图，可以在这里继续循环或者退出循环
            break;
        }
    }

    if (!bars)
    {
        bars = new QCPBars(xAxis, yAxis);
        // bars->setName(legendName); // 仅在创建新实例时设置名称
    }
    bars->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    // bars->setObjectName("Bars");                          // 设置objectName
    bars->setName(legendName); // 设置图例
    // bars->setPen(QPen(QColor(0, 160, 140).lighter(130))); // 设置柱状图的边框颜色
    // bars->setBrush(QColor(20, 68, 106));                  // 设置柱状图的画刷颜色

    if (curve_index == 1)
    {
        bars->setObjectName("NoisedBars");
        bars->setPen(QPen(QColor(0, 160, 140).lighter(130))); // 设置柱状图的边框颜色
        // bars->setBrush(QColor(20, 68, 106));                  // 设置柱状图的画刷颜色
        // 设置透明度
        bars->setBrush(QColor(20, 68, 106, 128));
    }
    else if (curve_index == 2)
    {
        bars->setObjectName("PureBars");
        bars->setPen(QPen(QColor(255, 0, 0).lighter(130))); // 设置柱状图的边框颜色
        // bars->setBrush(QColor(255, 0, 0));                  // 设置柱状图的画刷颜色
        // 设置透明度
        bars->setBrush(QColor(123, 104, 238, 72));
    }

    QCPRange x1Range = customPlot->xAxis->range();
    xAxis->setRange(x1Range.lower, x1Range.upper); // 设置x轴范围
    xAxis->setLabel("x");
    xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    QCPRange y1Range = customPlot->yAxis2->range();

    if (curve_index == 1)
    {
        yAxis->setRange(0, *maxElement); // 设置y轴范围
    }
    else if (curve_index == 2)
    {
        // 对比maxElement和y1Range.upper的大小，取最大值
        double y1Upper = std::max(*maxElement, y1Range.upper);
        yAxis->setRange(0, y1Upper); // 设置y轴范围
    }

    yAxis->setPadding(35); // 轴的内边距
    yAxis->setLabel("y");
    yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    if (curve_index == 1)
    {
        customPlot->yAxis2->setVisible(true);
        customPlot->yAxis2->setTickLabels(true);
        customPlot->yAxis2->setTickLabelColor(Qt::yellow);
        customPlot->yAxis2->setLabelColor(QColor(226, 60, 255));

        if (connection1 && connection2)
        {
            disconnect(connection1);
            disconnect(connection2);
        }

        // 等比例移动第二条坐标轴
        void (QCPAxis::*rangeChanged)(const QCPRange &, const QCPRange &) = &QCPAxis::rangeChanged;
        connection1 = connect(customPlot->yAxis,
                              rangeChanged,
                              customPlot->yAxis2,
                              [=](const QCPRange &newRange, const QCPRange &oldRange)
                              {
                                  QCPRange y2Range = customPlot->yAxis2->range();
                                  double dy = newRange.lower - oldRange.lower;
                                  double rate = dy / (oldRange.upper - oldRange.lower);
                                  double y2Lower = y2Range.lower + rate * (y2Range.upper - y2Range.lower);
                                  double y2Upper = y2Range.upper + rate * (y2Range.upper - y2Range.lower);
                                  customPlot->yAxis2->setRange(y2Lower, y2Upper);
                              });
        connection2 = connect(customPlot->xAxis,
                              rangeChanged,
                              customPlot->xAxis2,
                              [=](const QCPRange &newRange, const QCPRange &oldRange)
                              {
                                  QCPRange x2Range = customPlot->xAxis2->range();
                                  double dx = newRange.lower - oldRange.lower;
                                  double rate = dx / (oldRange.upper - oldRange.lower);
                                  double x2Lower = x2Range.lower + rate * (x2Range.upper - x2Range.lower);
                                  double x2Upper = x2Range.upper + rate * (x2Range.upper - x2Range.lower);
                                  customPlot->xAxis2->setRange(x2Lower, x2Upper);
                              });
    }

    // 设置宽度为4/5
    // bars->setWidthType(QCPBars::WidthType::wtPlotCoords);
    // bars->setWidth(1.2);
    bars->setWidthType(QCPBars::WidthType::wtPlotCoords);
    // bars->setWidth(0.75); // 根据间距的75%设置宽度，实现柱形图宽度与间隙宽度比例为3:1,实测是1：1，因此1.5是100%的宽度，1.125是75%的宽度
    // bars->setWidth(0.6); // 1.5*16（通道）= 24；即x轴的范围是24;若是32通道，则宽度设置为24/32=0.75时即为16通道的1.5；因此若要4：1.则应设置为0.75*4/5=0.6

    // 默认传递过来的xData的最后一个元素是柱形图宽度
    if (xData->size() == yData->size() + 1)
    {
        bars->setWidth((*xData)[xData->size() - 1]);
    }

    // // 设置透明度
    // bars->setBrush(QColor(20, 68, 106, 128));

    bars->setData(*xData, *yData);
    // 使bar切换到第二个坐标轴
    bars->setValueAxis(customPlot->yAxis2);
    bars->rescaleAxes(true);

    if (curve_index == 1)
    {

        // 在bars上显示对应的y值
        QCPBarsDataContainer *barsData = bars->data().data();
        // 如果原来界面上有valueLabels，先删除
        for (int i = 0; i < customPlot->layerCount(); ++i)
        {
            if (customPlot->layer(i)->name() == "valueLabels")
            {
                // 删除layer(i)层中的QCPItemText
                for (int j = 0; j < customPlot->layer(i)->children().size(); ++j)
                {
                    // 清除这个图层中的元素
                    customPlot->layer(i)->children().at(j)->setVisible(false);
                }
            }
        }
        for (int i = 0; i < barsData->size(); ++i)
        {
            const QCPBarsData *data = barsData->at(i);
            double x = data->key;
            double y = data->value;
            QString label = QString::number(y);
            customPlot->addLayer("valueLabels", nullptr, QCustomPlot::limAbove);
            QCPItemText *valueLabel = new QCPItemText(customPlot);
            valueLabel->setPositionAlignment(Qt::AlignCenter | Qt::AlignTop); // 对齐方式
            // 显示到bar的顶上方

            valueLabel->position->setType(QCPItemPosition::ptPlotCoords);
            valueLabel->position->setCoords(x, y);
            valueLabel->setText(label);
            // valueLabel->setFont(QFont(font().family(), 10));
            valueLabel->setPen(QPen(Qt::NoPen));
            // 背景设置为透明
            valueLabel->setBrush(QBrush(QColor(128, 128, 0, 0)));

            // 设置字体颜色
            valueLabel->setColor(QColor(255, 255, 255));
            // valueLabel->position->setCoords(x, y - 0.5); // Adjust the y-coordinate to position the label above the bar

            valueLabel->setLayer("valueLabels");
        }
    }

    // 显示bars
    bars->setVisible(true);
    customPlot->replot();
    // customPlot->graph(index)->setValueAxis(customPlot->yAxis2);
    // // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    // customPlot->graph(index)->rescaleAxes(true);
}

void CustomPlotManager::setLegendName(const QString &name, int curve_index)
{
    customPlot->graph(curve_index)->setName(name);
}

void CustomPlotManager::refreshPlot()
{
    customPlot->replot(QCustomPlot::rpQueuedReplot);
}

void CustomPlotManager::clearPlot()
{
    customPlot->clearGraphs();
    // initCustomPlotStyle();
    // customPlot->replot();
}

/**
 * @brief CustomPlotManager::hidePlot
 * 隐藏曲线显示
 */
void CustomPlotManager::hidePlot()
{
    stopGenerateTracerEventSignal();
    // 隐藏曲线显示
    for (int i = 0; i < customPlot->graphCount(); i++)
    {
        customPlot->graph(i)->setVisible(false);
    }
    for (int i = 0; i < customPlot->plottableCount(); ++i)
    {
        qDebug() << "plottable:" << customPlot->plottable(i)->name() << Qt::endl;
        if (customPlot->plottable(i)->objectName() == "NoisedBars")
        {
            customPlot->plottable(i)->setVisible(false);
            for (int i = 0; i < customPlot->layerCount(); ++i) // 隐藏valueLabels
            {
                if (customPlot->layer(i)->name() == "valueLabels")
                {
                    // 删除layer(i)层中的QCPItemText
                    for (int j = 0; j < customPlot->layer(i)->children().size(); ++j)
                    {
                        customPlot->layer(i)->children().at(j)->setVisible(false);
                    }
                }
            }
        }
        else if (customPlot->plottable(i)->objectName() == "PureBars")
        {
            customPlot->plottable(i)->setVisible(false);
        }
    }
    // 隐藏图例显示
    customPlot->legend->setVisible(false);
    // 隐藏跟踪点
    Singleton<TracerManager>::getInstance()->setTracerVisible(false);
    stopGenerateTracerEventSignal();
    customPlot->replot();
}

bool CustomPlotManager::showPlot()
{
    // 如果存在bar，则先显示
    for (int i = 0; i < customPlot->plottableCount(); ++i)
    {
        qDebug() << "show_plottable:" << customPlot->plottable(i)->objectName() << Qt::endl;
        if (customPlot->plottable(i)->objectName() == "NoisedBars")
        {
            customPlot->plottable(i)->setVisible(true);
            for (int i = 0; i < customPlot->layerCount(); ++i)
            {
                if (customPlot->layer(i)->name() == "valueLabels")
                {
                    // 删除layer(i)层中的QCPItemText
                    for (int j = 0; j < customPlot->layer(i)->children().size(); ++j)
                    {
                        customPlot->layer(i)->children().at(j)->setVisible(true);
                    }
                }
            }
        }
        else if (customPlot->plottable(i)->objectName() == "PureBars")
        {
            customPlot->plottable(i)->setVisible(true);
        }
    }
    // 显示曲线显示
    if (customPlot->graphCount() == 0)
    {
        return false;
    }
    for (int i = 0; i < customPlot->graphCount(); i++)
    {
        customPlot->graph(i)->setVisible(true);
        // 显示图例显示
        customPlot->legend->setVisible(true);
        // 显示跟踪点
        Singleton<TracerManager>::getInstance()->setTracerVisible(false);
        stopGenerateTracerEventSignal();
        // Singleton<TracerManager>::getInstance()->setTracerVisible(true);
        // startGenerateTracerEventSignal();
        customPlot->replot();
    }
    return true;
}

int CustomPlotManager::getCount()
{
    return customPlot->graphCount();
}

/**
 * @brief CustomPlotManager::getDataContainer
 * @param curve_index
 * @return
 * 获取QCustomPlot中的数据容器
 */
QSharedPointer<QCPGraphDataContainer> CustomPlotManager::getDataContainer(int curve_index)
{
    // qDebug() << "CustomPlotManager::getDataContainer:" << customPlot->graph(curve_index)->data().data() << Qt::endl;
    return customPlot->graph(curve_index)->data();
}

bool CustomPlotManager::getTracerStatus()
{
    return Singleton<TracerManager>::getInstance()->getTracerVisible();
}

/**
 * @brief CustomPlotManager::changeTracerStatus
 * @return
 * 改变跟踪点的状态
 */
bool CustomPlotManager::changeTracerStatus()
{
    bool visible = getTracerStatus();
    Singleton<TracerManager>::getInstance()->setTracerVisible(!visible);
    if (visible)
    {
        stopGenerateTracerEventSignal();
        refreshPlot();
    }
    else
    {
        startGenerateTracerEventSignal();
    }
    return !visible;
}

void CustomPlotManager::stopGenerateTracerEventSignal()
{
    disconnect(customPlot, &QCustomPlot::mouseMove, this, &CustomPlotManager::handleMouseMove);
    void (CustomPlotManager::*mouseMoveSignal)(QMouseEvent *event, QVector<QColor> colorVector) = &CustomPlotManager::mouseMoveSignal;
    void (TracerManager::*showTracer)(QMouseEvent *event, QVector<QColor> colorVector) = &TracerManager::showTracer;
    disconnect(this, mouseMoveSignal, Singleton<TracerManager>::getInstance(), showTracer);
}

void CustomPlotManager::startGenerateTracerEventSignal()
{
    connect(customPlot, &QCustomPlot::mouseMove, this, &CustomPlotManager::handleMouseMove);
    void (CustomPlotManager::*mouseMoveSignal)(QMouseEvent *event, QVector<QColor> colorVector) = &CustomPlotManager::mouseMoveSignal;
    void (TracerManager::*showTracer)(QMouseEvent *event, QVector<QColor> colorVector) = &TracerManager::showTracer;
    connect(this, mouseMoveSignal, Singleton<TracerManager>::getInstance(), showTracer);
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
    if (customPlot->yAxis2 != nullptr)
    {
        customPlot->yAxis2->setVisible(true);
        customPlot->yAxis2->setTickLabels(true);
        customPlot->yAxis2->setRange(lower, upper);
        customPlot->yAxis2->setTickLabelColor(Qt::yellow);
        customPlot->yAxis2->setLabelColor(QColor(226, 60, 255));
        // 设置第二条坐标轴可以拖动
        customPlot->yAxis2->setLabel(label);

        if (connection3) // 此处会产生问题，假如有多个地方调用此函数创建第二坐标轴，会导致connection3被覆盖，并解除上一次的连接
        {
            disconnect(connection3);
        }

        // 等比例移动第二条坐标轴
        void (QCPAxis::*rangeChanged)(const QCPRange &, const QCPRange &) = &QCPAxis::rangeChanged;
        connection3 = connect(customPlot->yAxis,
                              rangeChanged,
                              customPlot->yAxis2,
                              [=](const QCPRange &newRange, const QCPRange &oldRange)
                              {
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
