#include "manager/tracermanager.h"

TracerManager *TracerManager::tracerManagerInstance = nullptr;

TracerManager::TracerManager(QCustomPlot *customPlot, QObject *parent)
    : QObject(parent)
{
    //在构造的过程中虽然有new，但是最后释放了，所以不需要手动释放
    //在构造的过程中虽然有new，但是最后释放了，所以不需要手动释放

    m_TracerY = QSharedPointer<myTracer>(new myTracer(
        customPlot,
        customPlot->graph(0),
        DataTracer)); //这行代码是关键，设置鼠标跟踪器，DataTracer是枚举类型，可以设置为XAxisTracer，YAxisTracer，DataTracer
    //m_TracerX = QSharedPointer<myTracer> (new myTracer(CustomPlot, CustomPlot->graph(0), XAxisTracer));
    // connect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(showTracer(QMouseEvent *)));
    m_TracerY->setVisible(false);
}

TracerManager *TracerManager::getTracerManagerInstance(QCustomPlot *customPlot)
{
    if (!tracerManagerInstance) {
        tracerManagerInstance = new TracerManager(customPlot);
    }
    return tracerManagerInstance;
}

TracerManager *TracerManager::getTracerManagerInstance()
{
    if (!tracerManagerInstance) {
        // 在这里你可能想要抛出一个异常或者采取其他处理方式
        // 因为没有指定 QCustomPlot 的实例，单例模式无法正常工作
        return nullptr;
    }
    return tracerManagerInstance;
}

void TracerManager::destroyTracerManagerInstance()
{
    if (tracerManagerInstance) {
        delete tracerManagerInstance;
        tracerManagerInstance = nullptr;
    }
}

TracerManager::~TracerManager()
{
    m_TracerX.clear();
    m_TracerY.clear();
}

void TracerManager::setTracerCustomPlot(QCustomPlot *customPlot)
{
    m_TracerY.clear();
    m_TracerY = QSharedPointer<myTracer>(new myTracer(
        customPlot,
        customPlot->graph(0),
        DataTracer)); //这行代码是关键，设置鼠标跟踪器，DataTracer是枚举类型，可以设置为XAxisTracer，YAxisTracer，DataTracer
    m_TracerY->setVisible(false);
}

void TracerManager::setTracerVisible(bool visible)
{
    // m_TracerY->setVisible(visible);
    m_TracerY.data()->setVisible(visible);
}

bool TracerManager::getTracerVisible()
{
    return m_TracerY.data()->getVisible();
}

void TracerManager::showTracer(QMouseEvent *event, QVector<QColor> colorContainer)
{
    QCustomPlot *customPlot = m_TracerY.data()->getCustomPlot();
    double x = customPlot->xAxis->pixelToCoord(event->pos().x());
    double mouse_y = customPlot->yAxis->pixelToCoord(event->pos().y());

    QSharedPointer<QCPGraphDataContainer> tmpContainer;
    //获取customPlot的graph数量
    int graphCount = customPlot->graphCount();
    QVector<QSharedPointer<QCPGraphDataContainer>> tmpContainerVector;
    QVector<double> y;
    for (int i = 0; i < graphCount; ++i) {
        // tmpContainerVector.push_back(customPlot->graph(i)->data());
        tmpContainerVector.push_back(customPlot->graph(i)->data());
    }
    tmpContainer = customPlot->graph(0)->data();
    //使用二分法快速查找所在点数据！！！实测耗时与索引查找相当，二选一即可
    int low = 0, high = tmpContainer->size();
    // timer.start();
    while (high > low) {
        int middle = (low + high) / 2;
        if (x < tmpContainer->constBegin()->mainKey()
            || x > (tmpContainer->constEnd() - 1)->mainKey())
            break;

        if (x == (tmpContainer->constBegin() + middle)->mainKey()) {
            for (int j = 0; j < graphCount; ++j) {
                y.push_back(tmpContainerVector.at(j)->constBegin()->mainValue() + middle);
            }
            break;
        }
        if (x > (tmpContainer->constBegin() + middle)->mainKey()) {
            low = middle;
        } else if (x < (tmpContainer->constBegin() + middle)->mainKey()) {
            high = middle;
        }
        if (high - low <= 1) { //差值计算所在位置数据
            for (int j = 0; j < graphCount; ++j) {
                y.push_back((tmpContainerVector.at(j)->constBegin() + low)->mainValue()
                            + ((x - (tmpContainerVector.at(j)->constBegin() + low)->mainKey())
                               * ((tmpContainerVector.at(j)->constBegin() + high)->mainValue()
                                  - (tmpContainerVector.at(j)->constBegin() + low)->mainValue()))
                                  / ((tmpContainerVector.at(j)->constBegin() + high)->mainKey()
                                     - (tmpContainerVector.at(j)->constBegin() + low)->mainKey()));
            }
            break;
        }
    }
    // int x_index = 0;
    // if (x >= -12 && x <= 12) {
    //     x_index = static_cast<int>((x - (-12)) * 1e3);

    //     for (int j = 0; j < graphCount; ++j) {
    //         y.push_back((tmpContainerVector.at(j)->constBegin() + x_index)->mainValue());
    //     }
    // }
    //从y中获取与mouse_y最接近的数据
    double y_min = 0.0;
    int y_index = 0;
    if (y.size() != 0) {
        y_min = y.at(0);
        for (int i = 1; i < y.size(); ++i) {
            if (fabs(y.at(i) - mouse_y) < fabs(y_min - mouse_y)) {
                y_min = y.at(i);
                y_index = i;
            }
        }
    }
    //判断第y_index个曲线的y轴是否是第二个坐标轴
    // if (customPlot->graph(y_index)->valueAxis() == customPlot->yAxis2) {
    //     //根据第一个坐标轴和第二个坐标轴的比例，将y_min映射到第二个坐标轴上
    //     y_min = y_min * customPlot->yAxis->range().size() / customPlot->yAxis2->range().size();
    // }
    // 将上述if判断转换为三元表达式
    customPlot->graph(y_index)->valueAxis() == customPlot->yAxis2
        ? y_min = y_min
                  * static_cast<double>(customPlot->yAxis->range().size()
                                        / customPlot->yAxis2->range().size())
        : y_min = y_min;
    m_TracerY->updatePosition(x, y_min);

    m_TracerY->setPen(QPen(colorContainer.at(y_index), 3));
    m_TracerY->setBrush(QBrush(colorContainer.at(y_index)));
    m_TracerY->setLabelPen(QPen(colorContainer.at(y_index), 3));
    //使用三元表达式替换m_TracerY->setText(QString::number(x), QString::number(y_min));
    customPlot->graph(y_index)->valueAxis() == customPlot->yAxis2
        ? m_TracerY->setText(QString::number(x),
                             QString::number(
                                 y_min
                                 * static_cast<double>(customPlot->yAxis2->range().size()
                                                       / customPlot->yAxis->range().size())))
        : m_TracerY->setText(QString::number(x), QString::number(y_min));
    customPlot->replot();
}
