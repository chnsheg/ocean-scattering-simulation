#include "viewcontroller.h"
#include "buttongroupmanager.h"
#include "customplotmanager.h"
#include "show1buttongroupmanager.h"

ViewController *ViewController::viewControllerInstance = nullptr;

ViewController::ViewController(Ui::MainWindow *_ui, QWidget *parent)
    : QWidget(parent)
    , ui(_ui)
{
    //连接信号和槽
    connect(ButtonGroupsManager::getButtonGroupsManagerInstance(),
            SIGNAL(startButtonGroupClicked()),
            this,
            SLOT(startButtonClicked()));
    connect(ButtonGroupsManager::getButtonGroupsManagerInstance(),
            &ButtonGroupsManager::clearButtonGroupClicked,
            this,
            &ViewController::clearButtonClicked);
    connect(ButtonGroupsManager::getButtonGroupsManagerInstance(),
            &ButtonGroupsManager::tracerButtonGroupClicked,
            this,
            &ViewController::tracerButtonClicked);
    void (ViewController::*switchPlotPageButtonClicked)(int)
        = &ViewController::switchPlotPageButtonClicked;
    void (ButtonGroupsManager::*backButtonGroupClicked)(int)
        = &ButtonGroupsManager::backButtonGroupClicked;
    connect(ButtonGroupsManager::getButtonGroupsManagerInstance(),
            backButtonGroupClicked,
            this,
            switchPlotPageButtonClicked);

    void (ViewController::*switchShowPageButtonClicked)(int)
        = &ViewController::switchShowPageButtonClicked;
    void (Show1ButtonGroupManager::*show1ButtonGroupClicked)(int)
        = &Show1ButtonGroupManager::show1ButtonGroupClicked;
    connect(Show1ButtonGroupManager::getShow1ButtonGroupManagerInstance(),
            show1ButtonGroupClicked,
            this,
            switchShowPageButtonClicked);
}

//在此挂载视图层
ViewController *ViewController::getViewControllerInstance(Ui::MainWindow *_ui)
{
    //挂载ButtonGroups单例
    QVector<ButtonGroup> *buttonGroup = new QVector<ButtonGroup>;
    buttonGroup->push_back(ButtonGroup(_ui->show1, _ui->clear1, _ui->tracer1, _ui->back1));
    buttonGroup->push_back(ButtonGroup(_ui->show2, _ui->clear2, _ui->tracer2, _ui->back2));
    buttonGroup->push_back(ButtonGroup(_ui->show3, _ui->clear3, _ui->tracer3, _ui->back3));
    buttonGroup->push_back(ButtonGroup(_ui->show4, _ui->clear4, _ui->tracer4, _ui->back4));
    buttonGroup->push_back(ButtonGroup(_ui->show5, _ui->clear5, _ui->tracer5, _ui->back5));
    ButtonGroupsManager::getButtonGroupsManagerInstance(buttonGroup);
    //挂载show1ButtonGroupManager单例
    Show1ButtonGroup *show1ButtonGroup = new Show1ButtonGroup(_ui->homeButton_1,
                                                              _ui->homeButton_2,
                                                              _ui->homeButton_3,
                                                              _ui->homeButton_4,
                                                              _ui->homeButton_5);
    Show1ButtonGroupManager::getShow1ButtonGroupManagerInstance(show1ButtonGroup);

    //挂载CustomPlotManager单例
    CustomPlotManager::getCustomPlotManagerInstance(_ui->customPlot1);
    if (viewControllerInstance == nullptr)
        viewControllerInstance = new ViewController(_ui);
    return viewControllerInstance;
}

ViewController *ViewController::getViewControllerInstance()
{
    if (viewControllerInstance == nullptr)
        // 在这里你可能想要抛出一个异常或者采取其他处理方式
        // 因为没有指定 Ui::MainWindow 的实例，单例模式无法正常工作
        return nullptr;
    return viewControllerInstance;
}

void ViewController::destroyViewControllerInstance()
{
    if (viewControllerInstance != nullptr)
        delete viewControllerInstance;
}

int ViewController::getCurrentPageIndex()
{
    return ui->stackedWidget->currentIndex();
}

QCustomPlot *ViewController::getCurrentPageCustomPlot()
{
    int index = ui->stackedWidget->currentIndex(); //ui文件中对此命名产生的约束
    return ui->stackedWidget->findChild<QCustomPlot *>(QString("customPlot%1").arg(index));
}

void ViewController::updateViewStyleSlot(int plotInterfaceIndex)
{
    //判断plotInterfaceIndex是否为绘图界面,即是否在plotPageIndex中
    if (std::find(std::begin(plotPageIndex), std::end(plotPageIndex), plotInterfaceIndex)) {
        //更新视图层中CustomPlot的样式
        CustomPlotManager::getCustomPlotManagerInstance()->setCustomPlot(getCurrentPageCustomPlot());
        CustomPlotManager::getCustomPlotManagerInstance()->initCustomPlotStyle();
        //更新视图层中ButtonGroups的样式和状态
        ButtonGroupsManager::getButtonGroupsManagerInstance()->initButtonStyle(plotInterfaceIndex);
        ButtonGroupsManager::getButtonGroupsManagerInstance()->initButtonStatus(plotInterfaceIndex);
        Show1ButtonGroupManager::getShow1ButtonGroupManagerInstance()->initShow1ButtonGroupStyle();
        Show1ButtonGroupManager::getShow1ButtonGroupManagerInstance()->initShow1ButtonGroupStatus();
    } else if (std::find(std::begin(showPageIndex), std::end(showPageIndex), plotInterfaceIndex)) {
        // Update view style accordingly
        ui->stackedWidget->setCurrentIndex(plotInterfaceIndex);
        //更新视图层中ButtonGroups的样式和状态
        ButtonGroupsManager::getButtonGroupsManagerInstance()->initButtonStyle(plotInterfaceIndex);
        ButtonGroupsManager::getButtonGroupsManagerInstance()->initButtonStatus(plotInterfaceIndex);
    }
}

void ViewController::updateViewCurveSlot(const QVector<double> *xData,
                                         const QVector<double> *yData,
                                         int curve_index)
{
    // Update view curve accordingly
    int index = ui->stackedWidget->currentIndex();
    CustomPlotManager::getCustomPlotManagerInstance()->plotGraphToBuffer(xData, yData, curve_index);
    //根据index设定对应坐标轴样式，包括设置第二条坐标轴的范围和曲线的legend名称
    switch (index) {
    case 1:
        CustomPlotManager::getCustomPlotManagerInstance()->setLegendName("激光光谱", 0);
        CustomPlotManager::getCustomPlotManagerInstance()->refreshPlot();
        break;
    case 2:
        if (curve_index == 2) {
            CustomPlotManager::getCustomPlotManagerInstance()->setLegendName("布里渊散射曲线", 0);
            CustomPlotManager::getCustomPlotManagerInstance()->setLegendName("米散射曲线", 1);
            CustomPlotManager::getCustomPlotManagerInstance()->setLegendName("瑞利散射曲线", 2);
            CustomPlotManager::getCustomPlotManagerInstance()
                ->refreshPlot(); //在最后一条曲线绘制完毕后刷新
        }
        break;
    case 3:
        CustomPlotManager::getCustomPlotManagerInstance()->setLegendName("水下散射光谱", 0);
        CustomPlotManager::getCustomPlotManagerInstance()->refreshPlot();
        break;
    case 4:
        CustomPlotManager::getCustomPlotManagerInstance()->setLegendName("Fizeau仪器函数", 0);
        CustomPlotManager::getCustomPlotManagerInstance()->setLegendName("通过Fizeau后的光谱", 1);
        //判断是否需要创建第二个坐标轴
        if (curve_index == 0) {
            CustomPlotManager::getCustomPlotManagerInstance()->createSecondAxis(0, 1, "y2");
            CustomPlotManager::getCustomPlotManagerInstance()->switchToSecondAxis(0);
        } else if (curve_index == 1) {
            CustomPlotManager::getCustomPlotManagerInstance()->refreshPlot();
        }
        break;
    case 5:
        CustomPlotManager::getCustomPlotManagerInstance()->setLegendName("PMT能谱", 0);
        CustomPlotManager::getCustomPlotManagerInstance()->refreshPlot();
        break;
    default:
        break;
    }
}

void ViewController::updateViewClearSlot()
{
    int index = ui->stackedWidget->currentIndex();
    ButtonStatus ButtonWaitForOpen = {true, false, false}; // 等待开启显示的按钮状态结构体
    // Clear view curve accordingly
    CustomPlotManager::getCustomPlotManagerInstance()->hidePlot();
    ButtonGroupsManager::getButtonGroupsManagerInstance()->updateButtonStatus(index,
                                                                              ButtonWaitForOpen);
}

void ViewController::updateViewTracerSlot()
{
    // Update tracer button accordingly
    int index = ui->stackedWidget->currentIndex();
    bool isVisible = CustomPlotManager::getCustomPlotManagerInstance()->changeTracerStatus();
    // Update tracer button accordingly
    ButtonGroupsManager::getButtonGroupsManagerInstance()->updateTracerButtonText(index, isVisible);
}

void ViewController::updateViewPageSlot(int page_index)
{
    // Update view style accordingly
    ui->stackedWidget->setCurrentIndex(page_index);
    // 更新视图层的样式
    updateViewStyleSlot(page_index);
    // 更新视图层的按钮状态
}

void ViewController::startButtonClicked()
{
    if (CustomPlotManager::getCustomPlotManagerInstance()->getCount() == 0) {
        // Get input data from view
        InputDataListManager *inputDataList = new InputDataListManager();
        // for (int i = 0; i < ui.inputLineEdits.size(); ++i) {
        //     inputData.inputDataList->append(ui.inputLineEdits[i]->text());
        // }
        inputDataList->setInputDataList("0.8");
        inputDataList->setInputDataList("1000");
        inputDataList->setInputDataList("model1");
        // Send signal to controller
        emit onStartButtonClicked(inputDataList);
    } else {
        // Update view style accordingly
    }
    int index = ui->stackedWidget->currentIndex();
    ButtonStatus ButtonWaitForClose = {false, true, true};
    ButtonGroupsManager::getButtonGroupsManagerInstance()->updateButtonStatus(index,
                                                                              ButtonWaitForClose);
}

void ViewController::clearButtonClicked()
{
    emit onClearButtonClicked();
}
void ViewController::tracerButtonClicked()
{
    emit onTracerButtonClicked();
}

void ViewController::switchPlotPageButtonClicked(int index)
{
    // 从当前绘图界面退出
    //清除customPlot数据
    CustomPlotManager::getCustomPlotManagerInstance()->clearPlot();
    emit switchPageButtonClicked(showPageIndex[index - 1]);
}

void ViewController::switchShowPageButtonClicked(int index)
{
    // qDebug() << plotPageIndex[index - 1] << " ";
    //从当前展示界面退出
    emit switchPageButtonClicked(plotPageIndex[index - 1]);
}
