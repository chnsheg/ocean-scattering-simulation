#include "controller/plotcontroller.h"
#include "utils/logger.h"

PlotController *PlotController::plotControllerInstance = nullptr;

PlotController::PlotController(PlotView *_view, PageDataGenerator *_model, QObject *parent)
    : QObject(parent), view(_view), model(_model), rw(nullptr)
{
    void (PlotView::*onStartButtonClicked)(const int page_index) = &PlotView::onStartButtonClicked;
    connect(view, onStartButtonClicked, this, &PlotController::handleStartButtonClicked);
    connect(model, &PageDataGenerator::dataGenerated, this, &PlotController::handleDataGenerated);
    connect(view, &PlotView::onClearButtonClicked, this, &PlotController::handleClearButtonClicked);
    connect(view,
            &PlotView::onTracerButtonClicked,
            this,
            &PlotController::handleTracerButtonClicked);

    connect(view, &PlotView::switchPageButtonClicked, this, &PlotController::handleSwitchPageButtonClicked);

    connect(view, &PlotView::storeRuntimeDataSignal, this, &PlotController::handleStoreRuntimeDataSignal);

    connect(view, &PlotView::onSaveConstantButtonClicked, this, &PlotController::handleSaveConstantButtonClicked);

    connect(view, &PlotView::onImportConstantButtonClicked, this, &PlotController::handleImportConstantButtonClicked);

    connect(model, &PageDataGenerator::importConstantCompleted, this, &PlotController::handleImportConstantCompleted);

    connect(view, &PlotView::onDynamicButtonClicked, this, &PlotController::handleDynamicButtonClicked);

    connect(view, &PlotView::onShowButtonHover, this, &PlotController::handleShowButtonHover);

    connect(view, &PlotView::onShowButtonLeave, this, &PlotController::handleShowButtonLeave);
}

// TODO: Add destructor
PlotController::~PlotController()
{

    delete thread;
    delete rw;
    // delete dynamicView;
    for (int i = 0; i < dynamicViewVector.size(); ++i)
    {
        delete dynamicViewVector.at(i);
    }
    for (int i = 0; i < hoverInfoWidgetsVector.size(); ++i)
    {
        delete hoverInfoWidgetsVector.at(i);
    }
}

void PlotController::handleStartButtonClicked(int page_index)
{
    // 获取当前页面索引
    model->generatePairOfData(page_index);
}

/**
 * @brief 处理数据生成信号
 * @param xDataVector x轴数据
 * @param yDataVector y轴数据
 * @param curve_num 曲线数量
 */
void PlotController::handleDataGenerated(QVector<QVector<double> *> *xDataVector,
                                         QVector<QVector<double> *> *yDataVector,
                                         int curve_num)
{
    QVector<double> *xData;
    QVector<double> *yData;
    Singleton<Logger>::getInstance()->logMessage("Data generation completed!", Logger::Log);
    if (xDataVector->size() == 0 || yDataVector->size() == 0)
    {
        Singleton<Logger>::getInstance()->logMessage("Please set all input variables!",
                                                     Logger::Warning);
        return;
    }
    if (xDataVector->size() == yDataVector->size()) // 有多条非共用x轴数据的曲线需要绘制
    {
        for (int i = 0; i < xDataVector->size(); i++)
        {
            xData = xDataVector->at(i);
            yData = yDataVector->at(i);
            view->updateViewCurveSlot(xData, yData, i);
            // 释放内存，如果每次开始绘图导致系统卡顿，可以不释放内存，仅在切换页面时释放内存
            delete xData;
            delete yData;
            yData = nullptr;
            xData = nullptr;
            // yDataVector->replace(i, nullptr); // 设置为 nullptr 避免悬空指针
        }
    }
    else
    {
        xData = xDataVector->at(0); // 默认所有曲线的x轴数据都是一样的
        for (int i = 0; i < curve_num; i++)
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
    }
    delete xDataVector;
    xDataVector = nullptr;
    delete yDataVector;
    yDataVector = nullptr;
}

/**
 * @brief 处理存储运行时数据信号
 * @param dataContainer 数据容器
 * @param page_index 页面索引
 * @param curve_index 曲线索引
 */
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
        Singleton<Logger>::getInstance()->logMessage("Unknown signal sender!", Logger::Warning);
    }
}

/**
 * @brief 处理清除按钮点击事件
 */
void PlotController::handleClearButtonClicked()
{
    view->updateViewClearSlot();
}

/**
 * @brief 处理悬浮按钮点击事件
 */
void PlotController::handleTracerButtonClicked()
{
    view->updateViewTracerSlot();
}

/**
 * @brief 处理悬浮按钮悬停事件
 * @param index 按钮索引
 * @param pos 悬停位置
 */
void PlotController::handleShowButtonHover(int index, const QPoint &pos)
{
    HoverInfoWidget *hoverInfoWidget;
    int anchor = 0; // 用来锚定当前界面的hoverInfoWidget

    if (hoverInfoWidgetsOpened.contains(index))
    {
        // 查找index对应的hoverInfoWidgetsOpened中的索引
        anchor = hoverInfoWidgetsOpened.indexOf(index);
        hoverInfoWidget = hoverInfoWidgetsVector.at(anchor);
    }
    else
    {
        hoverInfoWidget = new HoverInfoWidget();
        hoverInfoWidgetsVector.append(hoverInfoWidget);
        hoverInfoWidgetsOpened.append(index);
    }

    QPixmap pixmapPtr;
    int result = model->getImageData(index, &pixmapPtr);
    if (result == -1)
    {
        hoverInfoWidgetsVector.removeOne(hoverInfoWidget);
        hoverInfoWidgetsOpened.removeOne(index);
        qDebug() << "获取图片失败！";
        return;
    }
    else if (result == 0)
    {
        // 显示一张空白图片
        hoverInfoWidget->setDisplayImage(QPixmap());
        qDebug() << "获取图片为空！";
    }
    else if (result == 1)
    {
        hoverInfoWidget->setDisplayImage(pixmapPtr);
        // 打印图片信息
        qDebug() << "pixmapPtr: " << pixmapPtr;
        qDebug() << "获取图片成功！";
    }

    // void HoverInfoWidget::setInfo(const QMap<QString, QVariant> &info)
    QMap<QString, QVariant> info;
    int infoDataResult = model->getInfoData(index, &info);
    if (infoDataResult == -1)
    {
        hoverInfoWidgetsVector.removeOne(hoverInfoWidget);
        hoverInfoWidgetsOpened.removeOne(index);
        qDebug() << "获取信息失败！";
        return;
    }
    else if (infoDataResult == 0)
    {
        // 显示一张空白图片
        hoverInfoWidget->setInfo(QMap<QString, QVariant>());
        qDebug() << "获取信息为空！";
    }
    else if (infoDataResult == 1)
    {
        hoverInfoWidget->setInfo(info);
        qDebug() << "获取信息成功！";
    }

    if (!hoverInfoWidget->getPinStatus())
    {

        hoverInfoWidget->move(pos.x(), pos.y() + 100);
    }
    // qDebug() << "pos: " << pos;
    hoverInfoWidget->showWithEffect();

    // hoverInfoWidget被关闭时，清除hoverInfoWidget的指针和opened列表中的索引
    connect(hoverInfoWidget, &HoverInfoWidget::closeHoverInfoWidgetSignal, this, [=]
            {
                qDebug() << "closeHoverInfoWidgetSignal";
        hoverInfoWidgetsVector.removeOne(hoverInfoWidget);
        hoverInfoWidgetsOpened.removeOne(index); });
}

/**
 * @brief 处理悬浮按钮离开事件
 * @param index 按钮索引
 */
void PlotController::handleShowButtonLeave(int index)
{
    int _index = index - Show1ButtonGroupId::leaveButton_1;
    // qDebug() << "ShowButtonLeave_index: " << _index;
    // qDebug() << "hoverInfoWidgetsOpened: " << hoverInfoWidgetsOpened;
    if (hoverInfoWidgetsOpened.contains(_index))
    {
        // qDebug() << "Is in hoverInfoWidgetsOpened!";
        int anchor = hoverInfoWidgetsOpened.indexOf(_index);
        HoverInfoWidget *hoverInfoWidget = hoverInfoWidgetsVector.at(anchor);
        hoverInfoWidget->hideWithEffect();
    }
}

/**
 * @brief 处理动态按钮点击事件
 * @param index 按钮索引
 */
void PlotController::handleDynamicButtonClicked(int index)
{
    // 通知model存储数据
    // QThread *thread;
    // DynamicPage *dynamicView;
    thread = new QThread();
    DynamicPage *dynamicView;
    int dynamicPageType = 0; // 0: 动态页面，1: 动态操作页面（模型反演）
    int anchor = 0;          // 用来锚定当前界面的dynamicView

    if (dynamicViewOpened.contains(index))
    {
        // 查找index对应的dynamicViewOpened中的索引
        anchor = dynamicViewOpened.indexOf(index);
        dynamicView = dynamicViewVector.at(anchor);
    }
    else
    {
        switch (index)
        {
        case 0:
            dynamicView = new DynamicPage(1);
            dynamicViewOpened.append(index);
            break;
        case 1:
            dynamicView = new DynamicPage(3);
            dynamicViewOpened.append(index);
            break;
        case 2:
            dynamicPageType = 1;
        }
        dynamicViewVector.append(dynamicView);
        anchor = dynamicViewVector.size() - 1;
    }
    if (dynamicPageType == 0)
    {
        // model->moveToThread(thread);
        // connect(thread, &QThread::started, this, [=]
        //         { model->generateDynamicData(index); });
        delete thread;
        model->generateDynamicData(index);

        connect(model, &PageDataGenerator::dynamicDataGenerated, dynamicView, &DynamicPage::updateDynamicView);

        connect(dynamicView, &DynamicPage::storeRuntimeDataSignal, this, &PlotController::handleStoreRuntimeDataSignal);

        // dynamicView被关闭时，清除动态页面的指针和opened列表中的索引
        connect(dynamicView, &DynamicPage::closeDynamicPageSignal, this, [=]
                {
        dynamicViewVector.removeOne(dynamicView);
        dynamicViewOpened.removeOne(index); });

        connection = connect(model, &PageDataGenerator::dataGenerateFinished, this, [=]
                             {
        this->dynamicViewVector.at(anchor)->show();
        // this->thread->quit();
        // this->thread->wait();
        // this->thread->deleteLater();
        disconnect(this->model,
                   &PageDataGenerator::dynamicDataGenerated,
                   this->dynamicViewVector.at(anchor),
                   &DynamicPage::updateDynamicView); 
                   disconnect(connection); });

        // thread->start();
    }
    else if (dynamicPageType == 1)
    {
        // model->moveToThread(thread);
        // connect(thread, &QThread::started, this, [=]
        //         { model->generateDynamicAction(0); });

        // connect(model, &PageDataGenerator::actionGenerateFinished, this, [=]
        //         {
        // // this->dynamicViewVector.at(index)->show();
        // this->thread->quit();
        // this->thread->wait();
        // this->thread->deleteLater(); });

        // thread->start();
        if (rw == nullptr)
        {
            rw = new RetrievalWindow();
        }

        rw->show();
        delete thread;
        // model->generateDynamicAction(0);
    }
}

/**
 * @brief 保存常量
 * @param index 常量组索引
 * @param save_type 保存类型
 */
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

/**
 * @brief 导入常量
 * @param index 常量组索引
 * @param save_type 保存类型
 */
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

/**
 * @brief 导入常量完成
 * @param page_index 页面索引
 * @param xDataVector x轴数据
 * @param yDataVector y轴数据
 */
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
        Singleton<Logger>::getInstance()->logMessage("The imported data is empty!", Logger::Warning);
        return;
    }
    else if (xDataVector->size() != yDataVector->size())
    {
        Singleton<Logger>::getInstance()->logMessage("Import data does not match!", Logger::Warning);
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

/**
 * @brief 切换页面
 * @param page_index 切换目标页面索引
 * @param area 截图区域
 * @param from_page_index 来源页面索引
 */

void PlotController::handleSwitchPageButtonClicked(int page_index, QRect area, int from_page_index)
{
    // 通知model截图
    if (area != QRect())
    {
        int index = view->getCurrentPageIndex() - 1;
        int captureResult = model->captureImageData(index, area);
        qDebug() << "page_index: " << index << " captureResult: " << captureResult;
    }

    // 先关闭所有的hoverInfoWidget
    // if (!view->showPageIndex.contains(page_index))
    // {
    //     // for (int i = 0; i < hoverInfoWidgetsVector.size(); ++i)
    //     // {
    //     //     hoverInfoWidgetsVector.at(i)->closeNow();
    //     // }

    //     // hoverInfoWidgetsVector.at(from_page_index)->closeNow();
    // }
    // 仅关闭切换至页面的hoverInfoWidget，可以保证悬浮窗更新
    int anchor = hoverInfoWidgetsOpened.indexOf(page_index - 1);

    if (page_index != 0 && anchor != -1)
    {
        hoverInfoWidgetsVector.at(anchor)->closeNow();
    }
    // 通知view切换页面，page_index从0开始
    view->updateViewPageSlot(page_index);
}
