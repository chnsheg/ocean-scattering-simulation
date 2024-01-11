#include "viewcontroller.h"
#include "customplotmanager.h"

ViewController *ViewController::viewControllerInstance = nullptr;

ViewController::ViewController(Ui::MainWindow *_ui, QWidget *parent)
    : QWidget(parent)
{
    ui = _ui;
    // Connect start button clicked signal

    for (int i = 0;
         i < ButtonGroupsManager::getButtonGroupsManagerInstance()->getButtonGroups()->size();
         i++) {
        void (ViewController::*startButtonClicked)() = &ViewController::startButtonClicked;
        connect(ButtonGroupsManager::getButtonGroupsManagerInstance()
                    ->getButtonGroups()
                    ->at(i)
                    .showButton,
                &QPushButton::clicked,
                this,
                startButtonClicked);
    }
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

    if (viewControllerInstance == nullptr)
        viewControllerInstance = new ViewController(_ui);

    //挂载CustomPlotManager单例
    CustomPlotManager::getCustomPlotManagerInstance(_ui->customPlot1);
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

void ViewController::updateViewStyle()
{
    // Update view style accordingly
    int index = ui->stackedWidget->currentIndex();
    //更新视图层中CustomPlot的样式
    CustomPlotManager::getCustomPlotManagerInstance()->setCustomPlot(getCurrentPageCustomPlot());
    CustomPlotManager::getCustomPlotManagerInstance()->initCustomPlotStyle();
    //更新视图层中ButtonGroups的样式和状态
    ButtonGroupsManager::getButtonGroupsManagerInstance()->initButtonStyle(index);
    ButtonGroupsManager::getButtonGroupsManagerInstance()->initButtonStatus(index);
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

void ViewController::updateButtonStatus(const ButtonStatus &status)
{
    int page_index = ui->stackedWidget->currentIndex();
    // Update button status accordingly
    ButtonGroupsManager::getButtonGroupsManagerInstance()->updateButtonStatus(page_index, status);
}

void ViewController::updateTracerButtonText(bool isVisible)
{
    int page_index = ui->stackedWidget->currentIndex();
    // Update tracer button accordingly
    ButtonGroupsManager::getButtonGroupsManagerInstance()->updateTracerButtonText(page_index,
                                                                                  isVisible);
}

void ViewController::startButtonClicked()
{
    // Get input data from view
    InputDataListManager inputDataList = InputDataListManager();
    // for (int i = 0; i < ui.inputLineEdits.size(); ++i) {
    //     inputData.inputDataList->append(ui.inputLineEdits[i]->text());
    // }
    // inputDataList.getInputDataList()->push_back("0.8");
    // inputDataList.getInputDataList()->push_back("1000");
    // inputDataList.getInputDataList()->push_back("model1");
    inputDataList.setInputDataList("0.8");
    inputDataList.setInputDataList("1000");
    inputDataList.setInputDataList("model1");
    // Send signal to controller
    emit startButtonClicked(inputDataList);
}
