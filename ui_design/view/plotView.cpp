#include "view/plotView.h"
#include "manager/customplotmanager.h"
#include "manager/lineeditgroupmanager.h"
#include "manager/texteditmanager.h"
#include "utils/logger.h"
#include "model/constantstorage.h"
#include "model/constantmap.h"
#include "view/dynamicview.h"

// PlotView *PlotView::plotViewInstance = nullptr;

PlotView::PlotView(Ui::MainWindow *_ui, QWidget *parent)
    : QWidget(parent), ui(_ui)
{
    // 挂载菜单项管理器单例
    Singleton<MenuManager>::getInstance(_ui->menuBar);

    // 挂载ButtonGroups单例
    QVector<ButtonGroup> *buttonGroup = new QVector<ButtonGroup>;
    buttonGroup->push_back(ButtonGroup(_ui->show1, _ui->clear1, _ui->tracer1, _ui->back1, _ui->clear1));
    buttonGroup->push_back(ButtonGroup(_ui->show2, _ui->clear2, _ui->tracer2, _ui->back2, _ui->clear2));
    buttonGroup->push_back(ButtonGroup(_ui->show3, _ui->clear3, _ui->tracer3, _ui->back3, _ui->clear3));
    buttonGroup->push_back(ButtonGroup(_ui->show4, _ui->clear4, _ui->tracer4, _ui->back4, _ui->clear4));
    buttonGroup->push_back(ButtonGroup(_ui->show5, _ui->clear5, _ui->tracer5, _ui->back5, _ui->clear5));
    buttonGroup->push_back(ButtonGroup(_ui->show6, _ui->clear6, _ui->tracer6, _ui->back6, _ui->clear6));
    buttonGroup->push_back(ButtonGroup(_ui->show7, _ui->clear7, _ui->tracer7, _ui->back7, _ui->clear7));
    buttonGroup->push_back(ButtonGroup(_ui->show8, _ui->clear8, _ui->tracer8, _ui->back8, _ui->clear8));

    // QPushButton *newButton = new QPushButton("新按钮"); // 创建新按钮
    // newButton->setObjectName("newButton");              // 给按钮设置一个对象名，方便以后引用

    // // 现在，我们需要将新按钮添加到widget1_3的布局中
    // // 首先获取到widget1_3的布局
    // QHBoxLayout *layout = qobject_cast<QHBoxLayout *>(ui->widget2_3->layout());
    // if (layout)
    // {                                 // 确保布局转换成功
    //     layout->addWidget(newButton); // 将新按钮添加到布局中
    // }

    // ButtonGroupsManager::getButtonGroupsManagerInstance(buttonGroup);
    Singleton<ButtonGroupsManager>::getInstance(buttonGroup);

    // 添加动态按钮
    Singleton<ButtonGroupsManager>::getInstance()->addDynamicButton(ui->widget2_3,
                                                                    "Laser broadening diagram");
    Singleton<ButtonGroupsManager>::getInstance()->addDynamicButton(ui->widget3_3,
                                                                    "extended analysis");
    Singleton<ButtonGroupsManager>::getInstance()->addDynamicButton(ui->widget5_3,
                                                                    "model inversion");

    // 挂载show1ButtonGroupManager单例
    Show1ButtonGroup *show1ButtonGroup = new Show1ButtonGroup(_ui->homeButton_1,
                                                              _ui->homeButton_2,
                                                              _ui->homeButton_3,
                                                              _ui->homeButton_4,
                                                              _ui->homeButton_5,
                                                              _ui->homeButton_6,
                                                              _ui->homeButton_7,
                                                              _ui->homeButton_8);
    Singleton<Show1ButtonGroupManager>::getInstance(show1ButtonGroup);
    _ui->homeButton_6->hide();
    _ui->homeButton_7->hide();
    _ui->homeButton_8->hide();

    // 挂载hoverInfoWidget单例
    // HoverInfoWidget *hoverInfoWidget = new HoverInfoWidget();
    // hoverInfoWidget->setAnchorPoint(QPoint(50, 50));
    // hoverInfoWidget->setDisplayImage(QPixmap(":/backgroundImage.png"));
    // QMap<QString, QVariant> info;
    // info.insert("Name", "Laser Spectroscopy");
    // info.insert("Description", "This is a laser spectroscopy system.");
    // info.insert("Author", "Yonghao");
    // hoverInfoWidget->setInfo(info);
    // hoverInfoWidget->showWithEffect();

    // 挂载CustomPlotManager单例
    //  Singleton<CustomPlotManager>::getInstance(_ui->customPlot1);
    Singleton<CustomPlotManager>::getInstance(_ui->customPlot1);

    // 初始化CustomPlot的list
    for (QCustomPlot *widgetName : {_ui->customPlot1, _ui->customPlot2, _ui->customPlot3, _ui->customPlot4, _ui->customPlot5, _ui->customPlot6, _ui->customPlot7, _ui->customPlot8})
    {
        customPlotList.append(widgetName);
    }

    // 挂载TextEditManager单例
    Singleton<TextEditManager>::getInstance(_ui->textEdit1);
    // 挂载Logger单例
    Singleton<Logger>::getInstance(Singleton<TextEditManager>::getInstance());
    // 挂载LineEditGroupManager单例
    QList<QList<QLineEdit *>> lineEditsList;
    QList<QList<QLabel *>> labelsList;
    for (auto &&widgetName : {_ui->cebianlan_1, _ui->cebianlan_2, _ui->cebianlan_3, _ui->cebianlan_4, _ui->cebianlan_5, _ui->cebianlan_6, _ui->cebianlan_7, _ui->cebianlan_8})
    {
        auto lineEdits = widgetName->findChildren<QLineEdit *>(QString(), Qt::FindDirectChildrenOnly);
        auto labels = widgetName->findChildren<QLabel *>(QString(), Qt::FindDirectChildrenOnly);
        std::sort(lineEdits.begin(), lineEdits.end(), [](QLineEdit *a, QLineEdit *b)
                  { return a->objectName() < b->objectName(); });
        std::sort(labels.begin(), labels.end(), [](QLabel *a, QLabel *b)
                  { return a->objectName() < b->objectName(); });
        lineEditsList.append(lineEdits);
        labelsList.append(labels);
    }
    Singleton<LineEditGroupManager>::getInstance(lineEditsList, labelsList);

    // 连接信号和槽
    void (MenuManager::*menuManagerEventSignal)(MenuActionId) = &MenuManager::eventSignal;
    void (PlotView::*handleMenuManager)(MenuActionId) = &PlotView::handleMenuManagerEvent;
    connect(Singleton<MenuManager>::getInstance(),
            menuManagerEventSignal,
            this,
            handleMenuManager);

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

    void (Show1ButtonGroupManager::*hoverSignal)(int, const QPoint &) = &Show1ButtonGroupManager::hoverSignal;
    void (PlotView::*handleShow1ButtonGroupHoverEvent)(int, const QPoint &) = &PlotView::handleShow1ButtonGroupHoverEvent;
    connect(Singleton<Show1ButtonGroupManager>::getInstance(),
            hoverSignal,
            this,
            handleShow1ButtonGroupHoverEvent);

    void (Show1ButtonGroupManager::*leaveSignal)(int) = &Show1ButtonGroupManager::leaveSignal;
    void (PlotView::*handleShow1ButtonGroupLeaveEvent)(int) = &PlotView::handleShow1ButtonGroupLeaveEvent;
    connect(Singleton<Show1ButtonGroupManager>::getInstance(),
            leaveSignal,
            this,
            handleShow1ButtonGroupLeaveEvent);
}

PlotView::PlotView() {} // 必须要在此处实现一个空的构造，否则会报错

PlotView::~PlotView() {}

int PlotView::getCurrentPageIndex()
{
    return ui->stackedWidget->currentIndex();
}

QList<QCustomPlot *> PlotView::getCurrentPageCustomPlot()
{
    QList<QCustomPlot *> currentPageCustomPlot;

    switch (getCurrentPageIndex())
    {
    case 1:
        currentPageCustomPlot.append(customPlotList[0]);
        break;
    case 2:
        currentPageCustomPlot.append(customPlotList[1]);
        break;
    case 3:
        currentPageCustomPlot.append(customPlotList[2]);
        break;
    case 4:
        currentPageCustomPlot.append(customPlotList[3]);
        break;
    case 5:
        currentPageCustomPlot.append(customPlotList[4]);
        break;
    case 6:
        currentPageCustomPlot.append(customPlotList[5]);
        break;
    case 7:
        currentPageCustomPlot.append(customPlotList[6]);
        break;
    case 8:
        currentPageCustomPlot.append(customPlotList[7]);
        break;
    default:
        break;
    }
    return currentPageCustomPlot;
}

QCustomPlot *PlotView::getCurrentAnchoredCustomPlot() // 根据锚点锚定当前页面的customPlot
{
    // int index = ui->stackedWidget->currentIndex(); // ui文件中对此命名产生的约束
    // return ui->stackedWidget->findChild<QCustomPlot *>(QString("customPlot%1").arg(index));
    int anchor = getCustomPlotManagerAnchor();
    return customPlotList[anchor - 1];
}

int PlotView::changeCustomPlotManagerAnchor(int index)
{
    Singleton<CustomPlotManager>::getInstance()->setCustomPlot(ui->stackedWidget->findChild<QCustomPlot *>(QString("customPlot%1").arg(index)));
    return getCustomPlotManagerAnchor(); // 用于切换锚定后返回
}

int PlotView::getCustomPlotManagerAnchor()
{
    QCustomPlot *customPlot = Singleton<CustomPlotManager>::getInstance()->getCustomPlot();
    int anchor = customPlotList.indexOf(customPlot) + 1;
    qDebug() << "customPlot index = " << anchor;
    return anchor;
}

void PlotView::updateViewStyleSlot(int plotInterfaceIndex)
{
    // 判断plotInterfaceIndex是否为绘图界面,即是否在plotPageIndex中
    if (plotPageIndex.contains(plotInterfaceIndex))
    {
        ui->stackedWidget->setCurrentIndex(plotInterfaceIndex);
        // 更新视图层中CustomPlot的样式
        for (auto &&customPlot : getCurrentPageCustomPlot())
        {
            Singleton<CustomPlotManager>::getInstance()->setCustomPlot(customPlot); // Tracer可能会出问题，只会锚定到最后一个customPlot上
        }
        // 默认开启customPlot显示
        bool existCurve = Singleton<CustomPlotManager>::getInstance()->showPlot();
        // 更新视图层中Menu的样式
        Singleton<MenuManager>::getInstance()->plotPageMenuStatus(existCurve);
        // Singleton<CustomPlotManager>::getInstance()->initCustomPlotStyle();
        // 更新视图层中TextEdit的样式
        Singleton<TextEditManager>::getInstance()->setTextEdit(
            ui->stackedWidget->findChild<QTextEdit *>(
                QString("textEdit%1").arg(getCurrentPageIndex())));
        Singleton<TextEditManager>::getInstance()->initTextEditStyle();
        // 输出logger信息
        switch (plotInterfaceIndex)
        {
        case 1:
            Singleton<Logger>::getInstance()->logMessage("Laser emission system", Logger::Title);
            break;
        case 2:
            Singleton<Logger>::getInstance()->logMessage("Scattering Spectral Generation System",
                                                         Logger::Title);
            break;
        case 3:
            Singleton<Logger>::getInstance()
                ->logMessage("Laser induced scattering spectroscopy generation system",
                             Logger::Title);
            break;
        case 4:
            Singleton<Logger>::getInstance()->logMessage("Fizeau interferometer system",
                                                         Logger::Title);
            break;
        case 5:
            Singleton<Logger>::getInstance()->logMessage("PMT system", Logger::Title);
            break;
        default:
            break;
        }
        // 更新视图层中ButtonGroups的样式和状态
        if (!existCurve)
        {
            Singleton<ButtonGroupsManager>::getInstance()->initButtonStyle(plotInterfaceIndex);
            Singleton<ButtonGroupsManager>::getInstance()->initButtonStatus(plotInterfaceIndex);
        }
        else
        {
            ButtonStatus ButtonWaitForClose = {true, true, true};
            Singleton<ButtonGroupsManager>::getInstance()->updateButtonStatus(plotInterfaceIndex, ButtonWaitForClose);
        }
        // 更新视图层中show1ButtonGroup的样式和状态
        // Singleton<Show1ButtonGroupManager>::getInstance()->initShow1ButtonGroupStyle();
        // Singleton<Show1ButtonGroupManager>::getInstance()->initShow1ButtonGroupStatus();
        Singleton<CustomPlotManager>::getInstance()->refreshPlot();
    }
    else if (showPageIndex.contains(plotInterfaceIndex))
    {
        Singleton<Logger>::getInstance()->logMessage("Return to the main interface!", Logger::Log);
        Singleton<MenuManager>::getInstance()->showPageMenuStatus();
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
    int anchor = getCustomPlotManagerAnchor();

    if (anchor != 5 || (anchor == 5 && curve_index == 0))
    {
        Singleton<CustomPlotManager>::getInstance()->plotGraphToBuffer(xData, yData, curve_index);
    }
    else if (anchor == 5 && curve_index == 1)
    {
        Singleton<CustomPlotManager>::getInstance()->plotBarGraphToBuffer(xData,
                                                                          yData,
                                                                          curve_index,
                                                                          "PMT with noise");
    }
    else if (anchor == 5 && curve_index == 2)
    {
        Singleton<CustomPlotManager>::getInstance()->plotBarGraphToBuffer(xData,
                                                                          yData,
                                                                          curve_index,
                                                                          "PMT without noise");
    }
    // 根据index设定对应坐标轴样式，包括设置第二条坐标轴的范围和曲线的legend名称
    switch (anchor)
    {
    case 1:
        Singleton<CustomPlotManager>::getInstance()->setLegendName("Laser spectrum", 0);
        Singleton<CustomPlotManager>::getInstance()->refreshPlot();
        Singleton<Logger>::getInstance()->logMessage("Laser spectrum drawing completed!",
                                                     Logger::Log);
        break;
    case 2:
        if (curve_index == 2)
        {
            Singleton<CustomPlotManager>::getInstance()->setLegendName("Brillouin curve", 0);
            Singleton<CustomPlotManager>::getInstance()->setLegendName("Rayleigh curve", 1);
            Singleton<CustomPlotManager>::getInstance()->setLegendName("Mie curve", 2);
            Singleton<CustomPlotManager>::getInstance()->refreshPlot(); // 在最后一条曲线绘制完毕后刷新
            Singleton<Logger>::getInstance()->logMessage("Scattering spectrum drawing completed!",
                                                         Logger::Log);
        }
        break;

    case 3:
        Singleton<CustomPlotManager>::getInstance()->setPenToRunFluently(curve_index);
        if (curve_index == 3)
        {
            Singleton<CustomPlotManager>::getInstance()->setLegendName("Brillouin curve", 0);
            Singleton<CustomPlotManager>::getInstance()->setLegendName("Rayleigh curve", 1);
            Singleton<CustomPlotManager>::getInstance()->setLegendName("Mie curve", 2);
            Singleton<CustomPlotManager>::getInstance()
                ->setLegendName("Underwater stimulated scattering spectra", 3);
            Singleton<CustomPlotManager>::getInstance()->refreshPlot();
            Singleton<Logger>::getInstance()
                ->logMessage("Laser induced scattering spectrum drawing completed", Logger::Log);
        }
        break;
    case 4:
        // 判断是否需要创建第二个坐标轴
        if (curve_index == 0)
        {
            Singleton<CustomPlotManager>::getInstance()->createSecondAxis(0, 1, "y2");
            Singleton<CustomPlotManager>::getInstance()->switchToSecondAxis(0);
            Singleton<Logger>::getInstance()
                ->logMessage("Fizeau interferometer function drawing completed", Logger::Log);
        }
        else if (curve_index == 1)
        {
            Singleton<CustomPlotManager>::getInstance()->setLegendName("Fizeau instrument function",
                                                                       0);
            Singleton<CustomPlotManager>::getInstance()->setLegendName("Spectrum after Fizeau", 1);
            Singleton<CustomPlotManager>::getInstance()->refreshPlot();
            Singleton<Logger>::getInstance()
                ->logMessage("The laser-induced scattering spectrum has been plotted using Fizeau",
                             Logger::Log);
        }
        break;
    case 5:
        if (curve_index == 0)
        {
            // 改变线宽，使得曲线绘制过程更加流畅
            Singleton<CustomPlotManager>::getInstance()->setPenToRunFluently(curve_index);
            Singleton<CustomPlotManager>::getInstance()->setLegendName("Original spectrum", 0);
            Singleton<CustomPlotManager>::getInstance()->refreshPlot();
            Singleton<Logger>::getInstance()->logMessage("PMT received spectrum drawing completed",
                                                         Logger::Log);
        }
        break;
    default:
        break;
    }

    if (anchor != 5 || curve_index == 0)
    {
        emit storeRuntimeDataSignal(Singleton<CustomPlotManager>::getInstance()->getDataContainer(curve_index), anchor - 1, curve_index);
    }
}

void PlotView::updateViewClearSlot()
{
    int index = ui->stackedWidget->currentIndex();
    ButtonStatus ButtonWaitForOpen = {true, false, false}; // 等待开启显示的按钮状态结构体
    // Clear view curve accordingly
    Singleton<CustomPlotManager>::getInstance()->hidePlot();
    Singleton<ButtonGroupsManager>::getInstance()->updateButtonStatus(index, ButtonWaitForOpen);
    Singleton<ButtonGroupsManager>::getInstance()->updateTracerButtonText(index, false);
    Singleton<Logger>::getInstance()->logMessage("隐藏曲线显示！", Logger::Log);
}

void PlotView::updateViewTracerSlot()
{
    // Update tracer button accordingly
    int index = ui->stackedWidget->currentIndex();
    // 更新Tracer状态s
    bool isVisible = Singleton<CustomPlotManager>::getInstance()->getTracerStatus();
    isVisible ? Singleton<Logger>::getInstance()->logMessage("关闭Tracer显示", Logger::Log) : Singleton<Logger>::getInstance()->logMessage("开启Tracer显示", Logger::Log);
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
    int page_index = ui->stackedWidget->currentIndex();
    Singleton<Logger>::getInstance()->logMessage("Start drawing...", Logger::Log);
    Singleton<CustomPlotManager>::getInstance()->clearPlot();
    Singleton<LineEditGroupManager>::getInstance()->saveLineEditGroupsText(page_index - 1);

    ButtonStatus ButtonWaitForClose = {true, true, true};
    Singleton<ButtonGroupsManager>::getInstance()->updateButtonStatus(page_index, ButtonWaitForClose);
    Singleton<MenuManager>::getInstance()->plotPageMenuStatus(true);

    // Singleton<CustomPlotManager>::getInstance()->clearPlot();
    emit onStartButtonClicked(page_index); // 只用告诉去读取哪个页面的数据就行了
}

void PlotView::handleMenuManagerEvent(MenuActionId menuActionId)
{
    int index = ui->stackedWidget->currentIndex();
    switch (menuActionId)
    {
    case Menu1_Menu1_Action1:
        Singleton<Logger>::getInstance()->logMessage("菜单1-1被点击", Logger::Log);
        saveConstantButtonClicked(index, 0);
        break;
    case Menu1_Menu1_Action2:
        Singleton<Logger>::getInstance()->logMessage("菜单1-2被点击", Logger::Log);
        saveConstantButtonClicked(index, 1);
        break;
    case Menu1_Menu2_Action1:
        Singleton<Logger>::getInstance()->logMessage("菜单2-1被点击", Logger::Log);
        saveConstantButtonClicked(index, 2);
        break;
    case Menu1_Menu2_Action2:
        Singleton<Logger>::getInstance()->logMessage("菜单2-2被点击", Logger::Log);
        saveConstantButtonClicked(index, 3);
        break;
    case Menu2_Menu1_Action1:
        Singleton<Logger>::getInstance()->logMessage("菜单3-1被点击", Logger::Log);
        importConstantButtonClicked(index, 0);
        break;
    case Menu2_Menu1_Action2:
        Singleton<Logger>::getInstance()->logMessage("菜单3-2被点击", Logger::Log);
        importConstantButtonClicked(index, 1);
        break;
    case Menu2_Menu2_Action1:
        Singleton<Logger>::getInstance()->logMessage("菜单4-1被点击", Logger::Log);
        importConstantButtonClicked(index, 2);
        break;
    case Menu2_Menu2_Action2:
        Singleton<Logger>::getInstance()->logMessage("菜单4-2被点击", Logger::Log);
        importConstantButtonClicked(index, 3);
        break;
    }
}

void PlotView::handleButtonGroupManagerEvent(ButtonGroupId buttonGroupId)
{
    // 在处理事件之前，重新锚定customPlotmanager和textEditManager
    for (auto &&customPlot : getCurrentPageCustomPlot())
    {
        Singleton<CustomPlotManager>::getInstance()->setCustomPlot(customPlot); // Tracer可能会出问题，只会锚定到最后一个customPlot上
    }
    Singleton<TextEditManager>::getInstance()->setTextEdit(
        ui->stackedWidget->findChild<QTextEdit *>(
            QString("textEdit%1").arg(getCurrentPageIndex())));
    Singleton<TextEditManager>::getInstance()->initTextEditStyle();

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
    case back6Button:
        switchPlotPageButtonClicked(0);
        break;
    case back7Button:
        switchPlotPageButtonClicked(0);
        break;
    case back8Button:
        switchPlotPageButtonClicked(0);
        break;
    case dynamic1Button:
        dynamicButtonClicked(0);
        break;
    case dynamic2Button:
        dynamicButtonClicked(1);
        break;
    case dynamic3Button:
        dynamicButtonClicked(2);
        break;
    }
}

void PlotView::handleShow1ButtonGroupHoverEvent(int button_index, const QPoint &pos)
{
    int page_index = button_index; // 将按键索引转换为页面索引
    qDebug() << "pos: " << pos;
    // 将pos转换为全局坐标
    QPoint globalPos = ui->stackedWidget->mapToGlobal(pos);
    emit onShowButtonHover(page_index, globalPos);
}

void PlotView::handleShow1ButtonGroupLeaveEvent(int button_index)
{
    int page_index = button_index; // 将按键索引转换为页面索引
    emit onShowButtonLeave(page_index);
}

void PlotView::saveConstantButtonClicked(int index, int save_type)
{
    if (save_type == 0)
    {
        Singleton<LineEditGroupManager>::getInstance()->saveLineEditGroupsText(index - 1);
    }
    else if (save_type == 1)
    {
        for (int i = 0; i < plotPageIndex.size(); i++)
        {
            Singleton<LineEditGroupManager>::getInstance()->saveLineEditGroupsText(i);
        }
    }

    emit onSaveConstantButtonClicked(index - 1, save_type);
}

void PlotView::importConstantButtonClicked(int index, int import_type)
{
    emit onImportConstantButtonClicked(index - 1, import_type);
}

void PlotView::clearButtonClicked()
{
    emit onClearButtonClicked();
}
void PlotView::tracerButtonClicked()
{
    emit onTracerButtonClicked();
}

void PlotView::dynamicButtonClicked(int index)
{
    // index表示是第几个动态按钮
    emit onDynamicButtonClicked(index);
}

void PlotView::switchPlotPageButtonClicked(int index)
{
    // 从当前绘图界面退出
    // 清除customPlot数据,由于与数据存储使用共享指针，因此不在此释放内存
    // Singleton<CustomPlotManager>::getInstance()->clearPlot();
    // Singleton<CustomPlotManager>::getInstance()->hidePlot();

    QRect globalRect = Singleton<CustomPlotManager>::getInstance()->getCustomPlotRect();

    // QRect Frame = ui->customPlot1->parentWidget()->geometry();
    // QPoint globalTopRight = mapToGlobal(Frame.topLeft());

    // qDebug() << "globalTopRight: " << globalTopRight;

    // int x_relative = globalTopRight.x();
    // int y_relative = globalTopRight.y();

    // 获取整个ui窗口的右上角坐标
    // int x = ui->customPlot1->parentWidget()->mapToGlobal(ui->customPlot1->pos()).x();
    // int y = ui->customPlot1->parentWidget()->mapToGlobal(ui->customPlot1->pos()).y();
    QWidget *widget = getCurrentAnchoredCustomPlot()->parentWidget();
    QCustomPlot *customPlot = getCurrentAnchoredCustomPlot();

    int x = widget->mapToGlobal(customPlot->pos()).x();
    int y = widget->mapToGlobal(customPlot->pos()).y();

    QPoint globalTopLeft = QPoint(x, y);
    qDebug() << "globalTopLeft: " << globalTopLeft;
    QRect paintRect = QRect(globalTopLeft, globalRect.size());

    emit switchPageButtonClicked(showPageIndex[index], paintRect, getCurrentPageIndex());
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
    case ShowButton_6:
        switchShowPageButtonClicked(5);
        break;
    case ShowButton_7:
        switchShowPageButtonClicked(6);
        break;
    case ShowButton_8:
        switchShowPageButtonClicked(7);
        break;
    }
}

void PlotView::switchShowPageButtonClicked(int index)
{
    // qDebug() << plotPageIndex[index - 1] << " ";
    // 从当前展示界面退出，进入绘图页面
    emit switchPageButtonClicked(plotPageIndex[index]);
}
