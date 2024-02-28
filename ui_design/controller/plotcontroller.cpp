#include "controller/plotcontroller.h"
#include "utils/logger.h"

PlotController *PlotController::plotControllerInstance = nullptr;

PlotController::PlotController(PlotView *_view, PageDataGenerator *_model, QObject *parent)
    : QObject(parent), view(_view), model(_model)
{
    void (PlotView::*onStartButtonClicked)(const int page_index) = &PlotView::onStartButtonClicked;
    connect(view, onStartButtonClicked, this, &PlotController::handleStartButtonClicked);
    connect(model, &PageDataGenerator::dataGenerated, this, &PlotController::handleDataGenerated);
    connect(view, &PlotView::onClearButtonClicked, this, &PlotController::handleClearButtonClicked);
    connect(view,
            &PlotView::onTracerButtonClicked,
            this,
            &PlotController::handleTracerButtonClicked);
    void (PlotView::*switchPageButtonClicked)(int) = &PlotView::switchPageButtonClicked;
    void (PlotController::*handleSwitchPageButtonClicked)(int) = &PlotController::handleSwitchPageButtonClicked;
    connect(view, switchPageButtonClicked, this, handleSwitchPageButtonClicked);

    connect(view, &PlotView::storeRuntimeDataSignal, this, &PlotController::handleStoreRuntimeDataSignal);
}

// TODO: Add destructor
PlotController::~PlotController() {}

// PlotController *PlotController::getPlotControllerInstance(PlotView *_view, PageDataGenerator *_model)
// {
//     if (plotControllerInstance == nullptr) {
//         plotControllerInstance = new PlotController(_view, _model);
//     }
//     return plotControllerInstance;
// }

// PlotController *PlotController::getPlotControllerInstance()
// {
//     if (plotControllerInstance != nullptr) {
//         return plotControllerInstance;
//     } else {
//         return nullptr;
//     }
// }

// void PlotController::destroyPlotControllerInstance()
// {
//     if (plotControllerInstance != nullptr) {
//         delete plotControllerInstance;
//         plotControllerInstance = nullptr;
//     }
// }

void PlotController::handleStartButtonClicked(int page_index)
{
    // 获取当前页面索引
    model->generatePairOfData(page_index);
}

void PlotController::handleDataGenerated(QVector<QVector<double> *> *xDataVector,
                                         QVector<QVector<double> *> *yDataVector,
                                         int curve_num)
{
    QVector<double> *xData;
    QVector<double> *yData;
    Singleton<Logger>::getInstance()->logMessage("数据生成完毕！", Logger::Log);
    if (xDataVector->size() == 0 || yDataVector->size() == 0)
    {
        Singleton<Logger>::getInstance()->logMessage("请设置所有输入变量！", Logger::Warning);
        view->updateViewCurveSlot(nullptr, nullptr, 0);
        return;
    }
    xData = xDataVector->at(0); // 默认所有曲线的x轴数据都是一样的
    for (int i = 0; i < curve_num; ++i)
    {
        yData = yDataVector->at(i);
        view->updateViewCurveSlot(xData, yData, i);
        // 释放内存，如果每次开始绘图导致系统卡顿，可以不释放内存，仅在切换页面时释放内存
        delete yData;
        yData = nullptr;
        // yDataVector->replace(i, nullptr); // 设置为 nullptr 避免悬空指针
    }
    delete xData;
    xData = nullptr;
    delete xDataVector;
    xDataVector = nullptr;
    delete yDataVector;
    yDataVector = nullptr;
}

void PlotController::handleStoreRuntimeDataSignal(QSharedPointer<QCPGraphDataContainer> dataContainer, const int page_index, const int curve_index)
{
    // 通知model存储数据
    model->storeRuntimeDataByIndex(dataContainer, page_index, curve_index);
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
    // 通知model存储数据
    view->updateViewPageSlot(page_index);
}
