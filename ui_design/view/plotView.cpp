#include "view/plotView.h"
#include "manager/customplotmanager.h"
#include "manager/texteditgroupmanager.h"

// PlotView *PlotView::plotViewInstance = nullptr;

PlotView::PlotView(Ui::MainWindow *_ui, QWidget *parent)
    : QWidget(parent), ui(_ui)
{
    // 挂载ButtonGroups单例
    QVector<ButtonGroup> *buttonGroup = new QVector<ButtonGroup>;
    buttonGroup->push_back(ButtonGroup(_ui->show1, _ui->clear1, _ui->tracer1, _ui->back1));
    buttonGroup->push_back(ButtonGroup(_ui->show2, _ui->clear2, _ui->tracer2, _ui->back2));
    buttonGroup->push_back(ButtonGroup(_ui->show3, _ui->clear3, _ui->tracer3, _ui->back3));
    buttonGroup->push_back(ButtonGroup(_ui->show4, _ui->clear4, _ui->tracer4, _ui->back4));
    buttonGroup->push_back(ButtonGroup(_ui->show5, _ui->clear5, _ui->tracer5, _ui->back5));
    // ButtonGroupsManager::getButtonGroupsManagerInstance(buttonGroup);
    Singleton<ButtonGroupsManager>::getInstance(buttonGroup);
    // 挂载show1ButtonGroupManager单例
    Show1ButtonGroup *show1ButtonGroup = new Show1ButtonGroup(_ui->homeButton_1,
                                                              _ui->homeButton_2,
                                                              _ui->homeButton_3,
                                                              _ui->homeButton_4,
                                                              _ui->homeButton_5);
    Singleton<Show1ButtonGroupManager>::getInstance(show1ButtonGroup);

    // 挂载CustomPlotManager单例
    //  Singleton<CustomPlotManager>::getInstance(_ui->customPlot1);
    Singleton<CustomPlotManager>::getInstance(_ui->customPlot1);
    // 挂载TextEditGroupManager单例
    QList<QList<QLineEdit *>> lineEditsList;
    for (auto &&widgetName : {_ui->cebianlan_1, _ui->cebianlan_2, _ui->cebianlan_3, _ui->cebianlan_4, _ui->cebianlan_5})
    {
        auto lineEdits = widgetName->findChildren<QLineEdit *>(QString(), Qt::FindDirectChildrenOnly);
        std::sort(lineEdits.begin(), lineEdits.end(), [](QLineEdit *a, QLineEdit *b)
                  { return a->objectName() < b->objectName(); });
        lineEditsList.append(lineEdits);
    }
    Singleton<LineEditGroupManager>::getInstance(lineEditsList);

    // 连接信号和槽
    void (ButtonGroupsManager::*buttonGroupsManagerEventSignal)(ButtonGroupId) = &ButtonGroupsManager::eventSignal;
    void (PlotView::*handleButtonGroupManager)(ButtonGroupId) = &PlotView::handleButtonGroupManagerEvent;
    connect(Singleton<ButtonGroupsManager>::getInstance(),
            buttonGroupsManagerEventSignal,
            this,
            handleButtonGroupManager);

    void (PlotView::*handleShow1ButtonGroupManagerEvent)(Show1ButtonGroupId) = &PlotView::handleShow1ButtonGroupManagerEvent;
    void (Show1ButtonGroupManager::*eventSignal)(Show1ButtonGroupId) = &Show1ButtonGroupManager::eventSignal;
    connect(Singleton<Show1ButtonGroupManager>::getInstance(),
            eventSignal,
            this,
            handleShow1ButtonGroupManagerEvent);
}

PlotView::PlotView() {} // 必须要在此处实现一个空的构造，否则会报错

PlotView::~PlotView() {}

int PlotView::getCurrentPageIndex()
{
    return ui->stackedWidget->currentIndex();
}

QCustomPlot *PlotView::getCurrentPageCustomPlot()
{
    int index = ui->stackedWidget->currentIndex(); // ui文件中对此命名产生的约束
    return ui->stackedWidget->findChild<QCustomPlot *>(QString("customPlot%1").arg(index));
}

void PlotView::updateViewStyleSlot(int plotInterfaceIndex)
{
    // 判断plotInterfaceIndex是否为绘图界面,即是否在plotPageIndex中
    if (plotPageIndex.contains(plotInterfaceIndex))
    {
        ui->stackedWidget->setCurrentIndex(plotInterfaceIndex);
        // 更新视图层中CustomPlot的样式
        Singleton<CustomPlotManager>::getInstance()->setCustomPlot(getCurrentPageCustomPlot());
        Singleton<CustomPlotManager>::getInstance()->initCustomPlotStyle();
        // 更新视图层中ButtonGroups的样式和状态
        Singleton<ButtonGroupsManager>::getInstance()->initButtonStyle(plotInterfaceIndex);
        Singleton<ButtonGroupsManager>::getInstance()->initButtonStatus(plotInterfaceIndex);
        // 更新视图层中show1ButtonGroup的样式和状态
        Singleton<Show1ButtonGroupManager>::getInstance()->initShow1ButtonGroupStyle();
        Singleton<Show1ButtonGroupManager>::getInstance()->initShow1ButtonGroupStatus();
        Singleton<CustomPlotManager>::getInstance()->refreshPlot();
    }
    else if (showPageIndex.contains(plotInterfaceIndex))
    {
        // Update view style accordingly
        ui->stackedWidget->setCurrentIndex(plotInterfaceIndex);
        // 更新视图层中ButtonGroups的样式和状态
        Singleton<Show1ButtonGroupManager>::getInstance()->initShow1ButtonGroupStyle();
        Singleton<Show1ButtonGroupManager>::getInstance()->initShow1ButtonGroupStatus();
    }
}

void PlotView::updateViewCurveSlot(const QVector<double> *xData,
                                   const QVector<double> *yData,
                                   int curve_index)
{
    // Update view curve accordingly
    int index = ui->stackedWidget->currentIndex();
    Singleton<CustomPlotManager>::getInstance()->plotGraphToBuffer(xData, yData, curve_index);
    // 根据index设定对应坐标轴样式，包括设置第二条坐标轴的范围和曲线的legend名称
    switch (index)
    {
    case 1:
        Singleton<CustomPlotManager>::getInstance()->setLegendName("激光光谱", 0);
        Singleton<CustomPlotManager>::getInstance()->refreshPlot();
        break;
    case 2:
        if (curve_index == 2)
        {
            Singleton<CustomPlotManager>::getInstance()->setLegendName("布里渊散射曲线", 0);
            Singleton<CustomPlotManager>::getInstance()->setLegendName("米散射曲线", 1);
            Singleton<CustomPlotManager>::getInstance()->setLegendName("瑞利散射曲线", 2);
            Singleton<CustomPlotManager>::getInstance()->refreshPlot(); // 在最后一条曲线绘制完毕后刷新
        }
        break;
    case 3:
        Singleton<CustomPlotManager>::getInstance()->setLegendName("水下散射光谱", 0);
        Singleton<CustomPlotManager>::getInstance()->refreshPlot();
        break;
    case 4:
        // 判断是否需要创建第二个坐标轴
        if (curve_index == 0)
        {
            Singleton<CustomPlotManager>::getInstance()->createSecondAxis(0, 1, "y2");
            Singleton<CustomPlotManager>::getInstance()->switchToSecondAxis(0);
        }
        else if (curve_index == 1)
        {
            Singleton<CustomPlotManager>::getInstance()->setLegendName("Fizeau仪器函数", 0);
            Singleton<CustomPlotManager>::getInstance()->setLegendName("通过Fizeau后的光谱", 1);
            Singleton<CustomPlotManager>::getInstance()->refreshPlot();
        }
        break;
    case 5:
        Singleton<CustomPlotManager>::getInstance()->setLegendName("PMT能谱", 0);
        Singleton<CustomPlotManager>::getInstance()->refreshPlot();
        break;
    default:
        break;
    }
    emit storeRuntimeDataSignal(Singleton<CustomPlotManager>::getInstance()->getDataContainer(curve_index), index, curve_index);
}

void PlotView::updateViewClearSlot()
{
    int index = ui->stackedWidget->currentIndex();
    ButtonStatus ButtonWaitForOpen = {true, false, false}; // 等待开启显示的按钮状态结构体
    // Clear view curve accordingly
    Singleton<CustomPlotManager>::getInstance()->hidePlot();
    Singleton<ButtonGroupsManager>::getInstance()->updateButtonStatus(index, ButtonWaitForOpen);
    Singleton<ButtonGroupsManager>::getInstance()->updateTracerButtonText(index, false);
}

void PlotView::updateViewTracerSlot()
{
    // Update tracer button accordingly
    int index = ui->stackedWidget->currentIndex();
    // 更新Tracer状态s
    bool isVisible = Singleton<CustomPlotManager>::getInstance()->getTracerStatus();

    // 更新按键状态
    Singleton<ButtonGroupsManager>::getInstance()->updateTracerButtonText(index, !isVisible);
    Singleton<CustomPlotManager>::getInstance()->refreshPlot();
    Singleton<CustomPlotManager>::getInstance()->changeTracerStatus();
}

void PlotView::updateViewPageSlot(int page_index)
{
    // 更新视图层的样式
    updateViewStyleSlot(page_index);
}

void PlotView::startButtonClicked()
{
    // if (Singleton<CustomPlotManager>::getInstance()->getCount() == 0)
    // {
    //     // Get input data from view
    //     // InputDataListManager *inputDataList = new InputDataListManager();
    //     // for (int i = 0; i < ui.inputLineEdits.size(); ++i) {
    //     //     inputData.inputDataList->append(ui.inputLineEdits[i]->text());
    //     // }
    //     // inputDataList->setInputDataList("0.8");
    //     // inputDataList->setInputDataList("1000");
    //     // inputDataList->setInputDataList("model1");
    //     // Send signal to controller
    //     int page_index = ui->stackedWidget->currentIndex();
    //     Singleton<LineEditGroupManager>::getInstance()->saveLineEditGroupsText(page_index - 1);
    //     emit onStartButtonClicked(page_index); // 只用告诉去读取哪个页面的数据就行了
    // }
    // else
    // {
    //     // Update view style accordingly
    //     Singleton<CustomPlotManager>::getInstance()->showPlot();
    //     bool visible = Singleton<CustomPlotManager>::getInstance()->getTracerStatus();
    //     qDebug() << "visible = " << visible;
    // }
    int page_index = ui->stackedWidget->currentIndex();
    Singleton<CustomPlotManager>::getInstance()->clearPlot();
    Singleton<LineEditGroupManager>::getInstance()->saveLineEditGroupsText(page_index - 1);
    emit onStartButtonClicked(page_index); // 只用告诉去读取哪个页面的数据就行了
    int index = ui->stackedWidget->currentIndex();
    ButtonStatus ButtonWaitForClose = {true, true, true};
    Singleton<ButtonGroupsManager>::getInstance()->updateButtonStatus(index, ButtonWaitForClose);
}

void PlotView::handleButtonGroupManagerEvent(ButtonGroupId buttonGroupId)
{
    switch (buttonGroupId)
    {
    case showButton:
        startButtonClicked();
        break;
    case clearButton:
        clearButtonClicked();
        break;
    case tracerButton:
        tracerButtonClicked();
        break;
    case back1Button:
        switchPlotPageButtonClicked(0);
        break;
    case back2Button:
        switchPlotPageButtonClicked(0);
        break;
    case back3Button:
        switchPlotPageButtonClicked(0);
        break;
    case back4Button:
        switchPlotPageButtonClicked(0);
        break;
    case back5Button:
        switchPlotPageButtonClicked(0);
        break;
    }
}

void PlotView::clearButtonClicked()
{
    emit onClearButtonClicked();
}
void PlotView::tracerButtonClicked()
{
    emit onTracerButtonClicked();
}

void PlotView::switchPlotPageButtonClicked(int index)
{
    // 从当前绘图界面退出
    // 清除customPlot数据,由于与数据存储使用共享指针，因此不在此释放内存
    // Singleton<CustomPlotManager>::getInstance()->clearPlot();
    Singleton<CustomPlotManager>::getInstance()->hidePlot();
    emit switchPageButtonClicked(showPageIndex[index]);
}

void PlotView::handleShow1ButtonGroupManagerEvent(Show1ButtonGroupId buttonGroupId)
{
    switch (buttonGroupId)
    {
    case ShowButton_1:
        switchShowPageButtonClicked(0);
        break;
    case ShowButton_2:
        switchShowPageButtonClicked(1);
        break;
    case ShowButton_3:
        switchShowPageButtonClicked(2);
        break;
    case ShowButton_4:
        switchShowPageButtonClicked(3);
        break;
    case ShowButton_5:
        switchShowPageButtonClicked(4);
        break;
    }
}

void PlotView::switchShowPageButtonClicked(int index)
{
    // qDebug() << plotPageIndex[index - 1] << " ";
    // 从当前展示界面退出
    emit switchPageButtonClicked(plotPageIndex[index]);
}
