#include "pageplot.h"
#include "customplotmanager.h"

PagePlot *PagePlot::pagePlotInstance = nullptr;

PagePlot::PagePlot(ViewController *_view, PageDataGenerator *_model, QObject *parent)
    : QObject(parent)
    , view(_view)
    , model(_model)
{
    void (ViewController::*startButtonClicked)(const InputDataListManager &)
        = &ViewController::startButtonClicked;
    connect(view, startButtonClicked, this, &PagePlot::handleStartButtonClicked);
    connect(model, &PageDataGenerator::dataGenerated, this, &PagePlot::handleDataGenerated);
}

// TODO: Add destructor
PagePlot::~PagePlot() {}

PagePlot *PagePlot::getPagePlotInstance(int _page_index)
{
    if (pagePlotInstance == nullptr) {
        pagePlotInstance = new PagePlot(ViewController::getViewControllerInstance(),
                                        PageDataGenerator::getPageDataGeneratorInstance());
    }
    return pagePlotInstance;
}

PagePlot *PagePlot::getPagePlotInstance()
{
    if (pagePlotInstance != nullptr) {
        return pagePlotInstance;
    } else {
        return nullptr;
    }
}

void PagePlot::destroyPagePlotInstance()
{
    if (pagePlotInstance != nullptr) {
        delete pagePlotInstance;
        pagePlotInstance = nullptr;
    }
}

void PagePlot::handleStartButtonClicked(const InputDataListManager &inputDataList)
{
    //获取当前页面索引
    int page_index = ViewController::getViewControllerInstance()->getCurrentPageIndex();
    //获取当前页面的数据
    switch (page_index) {
    case 1:
        model->generateData(, inputDataList);
        break;
    }

    // model->generateData(page_index, inputDataList);
}

void PagePlot::handleDataGenerated(const QVector<QVector<double> *> *xDataVector,
                                   const QVector<QVector<double> *> *yDataVector,
                                   const int curve_num)
{
    QVector<double> *xData;
    QVector<double> *yData;
    xData = xDataVector->at(0); //默认所有曲线的x轴数据都是一样的
    for (int i = 0; i < curve_num; ++i) {
        yData = yDataVector->at(i);
        view->updateViewCurveSlot(xData, yData, i);
    }
}

void PagePlot::switchPage()
{
    //更新视图层
    ViewController::getViewControllerInstance()->updateViewStyle();
    //更新控制层
    page_index = ViewController::getViewControllerInstance()->getCurrentPageIndex();
    customPlot = CustomPlotManager::getCustomPlotManagerInstance()->getCustomPlot();
}

void PagePlot::plotGraph(int count)
{
    customPlot->addGraph();
    auto maxElement = std::min_element(yData->begin(), yData->end(), [](double a, double b) {
        return (a >= b); //此处决定了应该将最大值更大的曲线放在最后再绘制
    });
    customPlot->xAxis->setRange(-12, 12);
    customPlot->yAxis->setRange(0, *maxElement);
    customPlot->graph(count)->setPen(QPen(colorContainer.at(count), 3)); //设置曲线颜色
    customPlot->graph(count)->setData(*xData, *yData);
    customPlot->replot(); //重绘 每次改变完以后都要调用这个进行重新绘制
}

void PagePlot::startPlot()
{
    xData = new QVector<double>;
    yData = new QVector<double>;
    //设置当前界面功能按键
    ViewController::getViewControllerInstance()->updateButtonStatus(ButtonWaitForClose);
    if (customPlot != nullptr) {
        customPlot->clearGraphs();
        switch (page_index) {
        case 1:
            //读取数据
            if ((xData->size() == 0 && yData->size() == 0)) {
                PageDataGenerator::generateData(PageDataGenerator::Frequence, xData);
                PageDataGenerator::generateData(PageDataGenerator::Laser, yData);
            }
            //绘制曲线
            plotGraph();
            customPlot->graph(0)->setName("激光光谱");
            clearData();
            break;
        case 2:
            if ((xData->size() == 0 && yData->size() == 0)) {
                PageDataGenerator::generateData(PageDataGenerator::Frequence, xData);
                PageDataGenerator::generateData(PageDataGenerator::BriScattering, yData);
            }
            plotGraph();
            customPlot->graph(0)->setName("布里渊散射曲线");
            clearData();

            if ((xData->size() == 0 && yData->size() == 0)) {
                PageDataGenerator::generateData(PageDataGenerator::Frequence, xData);
                PageDataGenerator::generateData(PageDataGenerator::RayScattering, yData);
            }
            plotGraph(1);
            customPlot->graph(1)->setName("瑞利散射曲线");
            clearData();

            if ((xData->size() == 0 && yData->size() == 0)) {
                PageDataGenerator::generateData(PageDataGenerator::Frequence, xData);
                PageDataGenerator::generateData(PageDataGenerator::MieScattering, yData);
            }
            plotGraph(2);
            customPlot->graph(2)->setName("米散射曲线");
            clearData();
            break;
        case 3:
            if ((xData->size() == 0 && yData->size() == 0)) {
                PageDataGenerator::generateData(PageDataGenerator::Frequence, xData);
                PageDataGenerator::generateData(PageDataGenerator::UnderwaterScattering, yData);
            }
            plotGraph();
            customPlot->graph(0)->setName("水下散射光谱");
            clearData();
            break;
        case 4:
            if ((xData->size() == 0 && yData->size() == 0)) {
                PageDataGenerator::generateData(PageDataGenerator::Frequence, xData);
                PageDataGenerator::generateData(PageDataGenerator::FizeauInstrument, yData);
            }
            plotGraph();
            customPlot->graph(0)->setName("Fizeau仪器函数");
            clearData();
            CustomPlotManager::getCustomPlotManagerInstance()->createSecondAxis(0, 1, "y");
            CustomPlotManager::getCustomPlotManagerInstance()->switchToSecondAxis(0);
            if ((xData->size() == 0 && yData->size() == 0)) {
                PageDataGenerator::generateData(PageDataGenerator::Frequence, xData);
                PageDataGenerator::generateData(PageDataGenerator::FizeauSpectra, yData);
            }
            plotGraph(1);
            customPlot->graph(1)->setName("通过Fizeau后的光谱");
            clearData();
            break;
        case 5:
            if ((xData->size() == 0 && yData->size() == 0)) {
                PageDataGenerator::generateData(PageDataGenerator::PMTFrequency, xData);
                PageDataGenerator::generateData(PageDataGenerator::PMTEnergy, yData);
            }
            plotGraph();
            customPlot->graph(0)->setName("PMT能谱");
            clearData();
            break;
        default:
            break;
        }
        //加载光标
        loadTracer();
        m_TracerY->setVisible(true);
        connect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(showTracer(QMouseEvent *)));
    }
}

void PagePlot::clearPlot()
{
    ViewController::getViewControllerInstance()->updateButtonStatus(ButtonWaitForOpen);
    ViewController::getViewControllerInstance()->updateTracerButtonText(true);
    disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(showTracer(QMouseEvent *)));
    m_TracerY->setVisible(false);
    freeData();
    customPlot->clearGraphs();
    customPlot->replot();
}

void PagePlot::loadTracer()
{
    //在构造的过程中虽然有new，但是最后释放了，所以不需要手动释放
    m_TracerY = QSharedPointer<myTracer>(new myTracer(
        customPlot,
        customPlot->graph(0),
        DataTracer)); //这行代码是关键，设置鼠标跟踪器，DataTracer是枚举类型，可以设置为XAxisTracer，YAxisTracer，DataTracer
}

void PagePlot::changeTracerStatus()
{
    bool isVisible = m_TracerY->getVisible();
    m_TracerY->setVisible(!isVisible);
    ViewController::getViewControllerInstance()->updateButtonStatus(isVisible ? ButtonWaitForOpen
                                                                              : ButtonWaitForClose);
    customPlot->replot();
}

void PagePlot::showTracer(QMouseEvent *event)
{
    // QElapsedTimer timer;
    // qint64 elapsedTime;
    double x = customPlot->xAxis->pixelToCoord(event->pos().x());
    double mouse_y = customPlot->yAxis->pixelToCoord(event->pos().y());
    //for(int i=0;i<1;i++)//ui->widget9->graph(0)->dataCount()
    //{
    // double y = 0;

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

/**
 * @brief 每次绘图完立即调用清除数据，避免内存泄漏
 */
void PagePlot::clearData()
{
    if (xData != nullptr) {
        xData->clear();
    }
    if (yData != nullptr) {
        yData->clear();
    }
}

void PagePlot::freeData()
{
    if (xData != nullptr) {
        delete xData;
        xData = nullptr;
    }
    if (yData != nullptr) {
        delete yData;
        yData = nullptr;
    }
}
