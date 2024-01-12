#include "pageplot.h"

PagePlot *PagePlot::pagePlotInstance = nullptr;

PagePlot::PagePlot(ViewController *_view, PageDataGenerator *_model, QObject *parent)
    : QObject(parent)
    , view(_view)
    , model(_model)
{
    void (ViewController::*onStartButtonClicked)(const InputDataListManager *)
        = &ViewController::onStartButtonClicked;
    connect(view, onStartButtonClicked, this, &PagePlot::handleStartButtonClicked);
    connect(model, &PageDataGenerator::dataGenerated, this, &PagePlot::handleDataGenerated);
    connect(view, &ViewController::onClearButtonClicked, this, &PagePlot::handleClearButtonClicked);
    connect(view,
            &ViewController::onTracerButtonClicked,
            this,
            &PagePlot::handleTracerButtonClicked);
    void (ViewController::*switchPageButtonClicked)(int) = &ViewController::switchPageButtonClicked;
    void (PagePlot::*handleSwitchPageButtonClicked)(int) = &PagePlot::handleSwitchPageButtonClicked;
    connect(view, switchPageButtonClicked, this, handleSwitchPageButtonClicked);
}

// TODO: Add destructor
PagePlot::~PagePlot() {}

PagePlot *PagePlot::getPagePlotInstance(ViewController *_view, PageDataGenerator *_model)
{
    if (pagePlotInstance == nullptr) {
        pagePlotInstance = new PagePlot(_view, _model);
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

void PagePlot::handleStartButtonClicked(const InputDataListManager *inputDataList)
{
    //获取当前页面索引
    int page_index = ViewController::getViewControllerInstance()->getCurrentPageIndex();
    model->generatePairOfData(page_index, inputDataList);
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
        //释放内存，如果每次开始绘图导致系统卡顿，可以不释放内存，仅在切换页面时释放内存
        delete yData;
        yData = nullptr;
    }
    delete xData;
    xData = nullptr;
    delete xDataVector;
    xDataVector = nullptr;
    delete yDataVector;
    yDataVector = nullptr;
}

void PagePlot::handleClearButtonClicked()
{
    view->updateViewClearSlot();
}

void PagePlot::handleTracerButtonClicked()
{
    view->updateViewTracerSlot();
}

void PagePlot::handleSwitchPageButtonClicked(int page_index)
{
    //获取当前页面索引
    qDebug() << "page_index = " << page_index;
    view->updateViewPageSlot(page_index);
}
