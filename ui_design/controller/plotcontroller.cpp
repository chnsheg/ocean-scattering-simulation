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

    connect(view, &PlotView::onSaveConstantButtonClicked, this, &PlotController::handleSaveConstantButtonClicked);

    connect(view, &PlotView::onImportConstantButtonClicked, this, &PlotController::handleImportConstantButtonClicked);

    connect(model, &PageDataGenerator::importConstantCompleted, this, &PlotController::handleImportConstantCompleted);

    connect(view, &PlotView::onDynamicButtonClicked, this, &PlotController::handleDynamicButtonClicked);
}

// TODO: Add destructor
PlotController::~PlotController() {}

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
    // // 通知model存储数据
    // model->storeRuntimeDataByIndex(dataContainer, page_index, curve_index);
    // 获取信号发送者
    QObject *sender = QObject::sender();
    if (sender == nullptr)
    {
        return;
    }
    // 获取发送者的类名
    QString senderClassName = sender->metaObject()->className();
    if (senderClassName == "PlotView")
    {
        model->storeRuntimeDataByIndex(dataContainer, page_index, curve_index);
    }
    else if (senderClassName == "DynamicPage")
    {
        model->storeRuntimeDataByIndex(dataContainer, page_index, curve_index, 1);
    }
    else
    {
        Singleton<Logger>::getInstance()->logMessage("未知的信号发送者！", Logger::Warning);
    }
}

void PlotController::handleClearButtonClicked()
{
    view->updateViewClearSlot();
}

void PlotController::handleTracerButtonClicked()
{
    view->updateViewTracerSlot();
}

void PlotController::handleDynamicButtonClicked(int index)
{
    // 通知model存储数据
    thread = new QThread;
    switch (index)
    {
    case 0:
        dynamicView = new DynamicPage(1);
        break;
    }

    model->moveToThread(thread);
    connect(thread, &QThread::started, this, [=]
            { model->generateDynamicData(index); });

    connect(model, &PageDataGenerator::dynamicDataGenerated, dynamicView, &DynamicPage::updateDynamicView);

    connect(dynamicView, &DynamicPage::storeRuntimeDataSignal, this, &PlotController::handleStoreRuntimeDataSignal);

    connect(model, &PageDataGenerator::dataGenerateFinished, this, [this]
            {
        dynamicView->show();
        thread->quit();
        thread->wait();
        thread->deleteLater(); });

    thread->start();
}

void PlotController::handleSaveConstantButtonClicked(int index, int save_type)
{
    switch (save_type)
    {
    case 0:
        model->storeConstantByGroupIndex(index);
        break;
    case 1:
        model->storeAllConstant();
        break;
    case 2:
        model->storeRuntimeDataByGroupIndex(index);
        break;
    case 3:
        model->storeAllRuntimeData();
        break;
    }
}

void PlotController::handleImportConstantButtonClicked(int index, int save_type)
{
    // 通知model存储数据
    // if (save_type == 0)
    // {
    //     model->storeConstantByGroupIndex(index);
    // }
    // else
    // {
    //     model->storeAllConstant();
    // }
    switch (save_type)
    {
    case 0:
        model->importConstantByGroupIndex(index);
        break;
    case 1:
        model->importAllConstant();
        break;
    case 2:
        model->importRuntimeDataByGroupIndex(index);
        break;
    case 3:
        model->importAllRuntimeData();
        break;
    }
}

void PlotController::handleImportConstantCompleted(const int page_index, const QVector<QVector<QVector<double> *> *> *xDataVector,
                                                   const QVector<QVector<QVector<double> *> *> *yDataVector)
{
    QVector<double> *xData;
    QVector<double> *yData;
    QVector<QVector<double> *> *xDataContainer;
    QVector<QVector<double> *> *yDataContainer;
    int anchor = 0; // 用来锚定当前界面的customPlotManager

    if (xDataVector->size() == 0 || yDataVector->size() == 0)
    {
        Singleton<Logger>::getInstance()->logMessage("导入数据为空！", Logger::Warning);
        return;
    }
    else if (xDataVector->size() != yDataVector->size())
    {
        Singleton<Logger>::getInstance()->logMessage("导入数据不匹配！", Logger::Warning);
        return;
    }
    else if (xDataVector->size() == 1 && yDataVector->size() == 1)
    {
        xDataContainer = xDataVector->at(0);
        yDataContainer = yDataVector->at(0);
        anchor = view->changeCustomPlotManagerAnchor(page_index + 1);
        for (int i = 0; i < xDataVector->size(); ++i)
        {
            xData = xDataContainer->at(i);
            yData = yDataContainer->at(i);
            view->updateViewCurveSlot(xData, yData, i);
            // 释放内存，如果每次开始绘图导致系统卡顿，可以不释放内存，仅在切换页面时释放内存
            delete xData;
            delete yData;
            yData = nullptr;
            xData = nullptr;
            // yDataVector->replace(i, nullptr); // 设置为 nullptr 避免悬空指针
        }
        view->changeCustomPlotManagerAnchor(anchor); // 恢复当前页面的customPlotManager
        delete xDataContainer;
        xDataContainer = nullptr;
        delete yDataContainer;
        yDataContainer = nullptr;
    }
    else if (xDataVector->size() > 1 && yDataVector->size() > 1)
    {
        for (int i = 0; i < xDataVector->size(); ++i)
        {
            xDataContainer = xDataVector->at(i);
            yDataContainer = yDataVector->at(i);
            if (i == 0)
            {
                anchor = view->changeCustomPlotManagerAnchor(i + 1);
            }
            else
            {
                view->changeCustomPlotManagerAnchor(i + 1);
            }
            for (int j = 0; j < xDataContainer->size(); ++j)
            {
                xData = xDataContainer->at(j);
                yData = yDataContainer->at(j);
                view->updateViewCurveSlot(xData, yData, j);
                // 释放内存，如果每次开始绘图导致系统卡顿，可以不释放内存，仅在切换页面时释放内存
                delete xData;
                delete yData;
                yData = nullptr;
                xData = nullptr;
                // yDataVector->replace(j, nullptr); // 设置为 nullptr 避免悬空指针
            }
            delete xDataContainer;
            xDataContainer = nullptr;
            delete yDataContainer;
            yDataContainer = nullptr;
        }
        view->changeCustomPlotManagerAnchor(anchor); // 恢复当前页面的customPlotManager
        delete xDataVector;
        xDataVector = nullptr;
        delete yDataVector;
        yDataVector = nullptr;
    }
}

void PlotController::handleSwitchPageButtonClicked(int page_index)
{
    // 通知model存储数据
    view->updateViewPageSlot(page_index);
}
