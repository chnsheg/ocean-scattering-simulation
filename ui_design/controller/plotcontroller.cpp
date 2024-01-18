#include "controller/plotcontroller.h"

PlotController *PlotController::plotControllerInstance = nullptr;

PlotController::PlotController(PlotView *_view, PageDataGenerator *_model, QObject *parent)
    : QObject(parent)
    , view(_view)
    , model(_model)
{
    void (PlotView::*onStartButtonClicked)(const InputDataListManager *)
        = &PlotView::onStartButtonClicked;
    connect(view, onStartButtonClicked, this, &PlotController::handleStartButtonClicked);
    connect(model, &PageDataGenerator::dataGenerated, this, &PlotController::handleDataGenerated);
    connect(view, &PlotView::onClearButtonClicked, this, &PlotController::handleClearButtonClicked);
    connect(view,
            &PlotView::onTracerButtonClicked,
            this,
            &PlotController::handleTracerButtonClicked);
    void (PlotView::*switchPageButtonClicked)(int) = &PlotView::switchPageButtonClicked;
    void (PlotController::*handleSwitchPageButtonClicked)(int)
        = &PlotController::handleSwitchPageButtonClicked;
    connect(view, switchPageButtonClicked, this, handleSwitchPageButtonClicked);
}

// TODO: Add destructor
PlotController::~PlotController() {}

PlotController *PlotController::getPlotControllerInstance(PlotView *_view, PageDataGenerator *_model)
{
    if (plotControllerInstance == nullptr) {
        plotControllerInstance = new PlotController(_view, _model);
    }
    return plotControllerInstance;
}

PlotController *PlotController::getPlotControllerInstance()
{
    if (plotControllerInstance != nullptr) {
        return plotControllerInstance;
    } else {
        return nullptr;
    }
}

void PlotController::destroyPlotControllerInstance()
{
    if (plotControllerInstance != nullptr) {
        delete plotControllerInstance;
        plotControllerInstance = nullptr;
    }
}

void PlotController::handleStartButtonClicked(const InputDataListManager *inputDataList)
{
    //获取当前页面索引
    int page_index = PlotView::getPlotViewInstance()->getCurrentPageIndex();
    model->generatePairOfData(page_index, inputDataList);
}

void PlotController::handleDataGenerated(const QVector<QVector<double> *> *xDataVector,
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

void PlotController::handleClearButtonClicked()
{
    view->updateViewClearSlot();
}

void PlotController::handleTracerButtonClicked()
{
    view->updateViewTracerSlot();
}

void PlotController::handleSwitchPageButtonClicked(int page_index)
{
    //获取当前页面索引
    qDebug() << "page_index = " << page_index;
    view->updateViewPageSlot(page_index);
}
