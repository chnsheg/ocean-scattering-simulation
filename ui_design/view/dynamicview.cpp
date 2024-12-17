#include "dynamicview.h"
#include "model/constantmap.h"
#include "model/constantstorage.h"

int DynamicPage::dynamicPageObjectNum = 0; // 初始化静态成员变量

DynamicPage::DynamicPage(int pageCount, QWidget *parent)
    : QWidget(parent), pageCount(pageCount)
{
    plots = new QVector<QCustomPlot *>(); // 初始化QCustomPlot容器
    setupUi();
    setAttribute(Qt::WA_DeleteOnClose);
    pageObjectId = dynamicPageObjectNum++;
}

// 在页面关闭时，释放QCustomPlot容器
DynamicPage::~DynamicPage()
{
    for (int i = 0; i < plots->size(); ++i)
    {
        delete (*plots)[i];
    }
    delete plots;
    delete stackedWidget;
    delete prevPageButton;
    delete nextPageButton;
    delete showCursorButton;
    delete closeButton;

    dynamicPageObjectNum--;

    emit closeDynamicPageSignal(pageObjectId);

    // 关闭窗口
    this->close();

    qDebug() << "DynamicPage object deleted";
}

void DynamicPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);

    for (int i = 0; i < pageCount + 1; ++i) {
        createPage(i);
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    prevPageButton = new QPushButton("Previous page", this);
    nextPageButton = new QPushButton("Next page", this);
    showCursorButton = new QPushButton("Show cursor", this);
    closeButton = new QPushButton("close", this);

    buttonLayout->addWidget(prevPageButton);
    buttonLayout->addWidget(nextPageButton);
    buttonLayout->addWidget(showCursorButton);
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);

    connect(prevPageButton, &QPushButton::clicked, this, &DynamicPage::onPrevPageClicked);
    connect(nextPageButton, &QPushButton::clicked, this, &DynamicPage::onNextPageClicked);
    connect(showCursorButton, &QPushButton::clicked, this, &DynamicPage::onShowCursorClicked);
    connect(closeButton, &QPushButton::clicked, this, &DynamicPage::onCloseClicked);

    prevPageButton->setEnabled(pageCount > 1);
    nextPageButton->setEnabled(pageCount > 1);

    //     /**正常情况下样式**/
    // #stackedWidget QPushButton{
    //     position: absolute;
    //     top: 50%;
    //     left: 50%;

    // 	font: 28px "微软雅黑";
    //     font-weight: 500;
    //     color: #FFFFFF;
    //     width: 150px;
    //     height: 60px;
    //     background-color: rgba(52, 73, 94, 1);
    //     border-color: #2f3640;
    //     border-radius: 10px;
    //     border-style: solid;
    //     border-width: 2px;
    //     /* padding: 2px; */
    //     margin-left: auto;
    //     margin-top: 10px;
    // }

    // /**鼠标停留在按钮上的样式**/
    // #stackedWidget QPushButton:hover{
    //     color: rgba(52, 73, 94, 1);
    //     background-color: #FFFFFF;
    //     border-color: #2f3640;
    // }

    // /**鼠标按压下去的样式**/
    // #stackedWidget QPushButton::pressed,QPushButton:checked{
    //     color: #FFFFFF;
    //     background-color: qlineargradient(spread pad, x1 0, y1 0, x2 0, y2 1, stop 0 #273c75, stop 1 #487eb0);
    // }

    // /**按钮失能情况下样式**/
    // #stackedWidget QPushButton:disabled{
    //     color: #2f3640;
    //     background-color: #dcdde1;
    //     border-color: #dcdde1;
    // }
    prevPageButton->setStyleSheet("QPushButton{"
                                  "position: absolute;"
                                  "top: 50%;"
                                  "left: 50%;"
                                  "font: 28px \"微软雅黑\";"
                                  "font-weight: 500;"
                                  "color: #FFFFFF;"
                                  "width: 150px;"
                                  "height: 60px;"
                                  "background-color: rgba(52, 73, 94, 1);"
                                  "border-color: #2f3640;"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "border-width: 2px;"
                                  "margin-left: auto;"
                                  "margin-top: 10px;"
                                  "}"
                                  "QPushButton:hover{"
                                  "color: rgba(52, 73, 94, 1);"
                                  "background-color: #FFFFFF;"
                                  "border-color: #2f3640;"
                                  "}"
                                  "QPushButton::pressed,QPushButton:checked{"
                                  "color: #FFFFFF;"
                                  "background-color: qlineargradient(spread pad, x1 0, y1 0, x2 0, y2 1, stop 0 #273c75, stop 1 #487eb0);"
                                  "}"
                                  "QPushButton:disabled{"
                                  "color: #2f3640;"
                                  "background-color: #dcdde1;"
                                  "border-color: #dcdde1;"
                                  "}"); // 设置样式

    nextPageButton->setStyleSheet("QPushButton{"
                                  "position: absolute;"
                                  "top: 50%;"
                                  "left: 50%;"
                                  "font: 28px \"微软雅黑\";"
                                  "font-weight: 500;"
                                  "color: #FFFFFF;"
                                  "width: 150px;"
                                  "height: 60px;"
                                  "background-color: rgba(52, 73, 94, 1);"
                                  "border-color: #2f3640;"
                                  "border-radius: 10px;"
                                  "border-style: solid;"
                                  "border-width: 2px;"
                                  "margin-left: auto;"
                                  "margin-top: 10px;"
                                  "}"
                                  "QPushButton:hover{"
                                  "color: rgba(52, 73, 94, 1);"
                                  "background-color: #FFFFFF;"
                                  "border-color: #2f3640;"
                                  "}"
                                  "QPushButton::pressed,QPushButton:checked{"
                                  "color: #FFFFFF;"
                                  "background-color: qlineargradient(spread pad, x1 0, y1 0, x2 0, y2 1, stop 0 #273c75, stop 1 #487eb0);"
                                  "}"
                                  "QPushButton:disabled{"
                                  "color: #2f3640;"
                                  "background-color: #dcdde1;"
                                  "border-color: #dcdde1;"
                                  "}"); // 设置样式

    showCursorButton->setStyleSheet("QPushButton{"
                                    "position: absolute;"
                                    "top: 50%;"
                                    "left: 50%;"
                                    "font: 28px \"微软雅黑\";"
                                    "font-weight: 500;"
                                    "color: #FFFFFF;"
                                    "width: 150px;"
                                    "height: 60px;"
                                    "background-color: rgba(52, 73, 94, 1);"
                                    "border-color: #2f3640;"
                                    "border-radius: 10px;"
                                    "border-style: solid;"
                                    "border-width: 2px;"
                                    "margin-left: auto;"
                                    "margin-top: 10px;"
                                    "}"
                                    "QPushButton:hover{"
                                    "color: rgba(52, 73, 94, 1);"
                                    "background-color: #FFFFFF;"
                                    "border-color: #2f3640;"
                                    "}"
                                    "QPushButton::pressed,QPushButton:checked{"
                                    "color: #FFFFFF;"
                                    "background-color: qlineargradient(spread pad, x1 0, y1 0, x2 0, y2 1, stop 0 #273c75, stop 1 #487eb0);"
                                    "}"
                                    "QPushButton:disabled{"
                                    "color: #2f3640;"
                                    "background-color: #dcdde1;"
                                    "border-color: #dcdde1;"
                                    "}"); // 设置样式

    closeButton->setStyleSheet("QPushButton{"
                               "position: absolute;"
                               "top: 50%;"
                               "left: 50%;"
                               "font: 28px \"微软雅黑\";"
                               "font-weight: 500;"
                               "color: #FFFFFF;"
                               "width: 150px;"
                               "height: 60px;"
                               "background-color: rgba(52, 73, 94, 1);"
                               "border-color: #2f3640;"
                               "border-radius: 10px;"
                               "border-style: solid;"
                               "border-width: 2px;"
                               "margin-left: auto;"
                               "margin-top: 10px;"
                               "}"
                               "QPushButton:hover{"
                               "color: rgba(52, 73, 94, 1);"
                               "background-color: #FFFFFF;"
                               "border-color: #2f3640;"
                               "}"
                               "QPushButton::pressed,QPushButton:checked{"
                               "color: #FFFFFF;"
                               "background-color: qlineargradient(spread pad, x1 0, y1 0, x2 0, y2 1, stop 0 #273c75, stop 1 #487eb0);"
                               "}"
                               "QPushButton:disabled{"
                               "color: #2f3640;"
                               "background-color: #dcdde1;"
                               "border-color: #dcdde1;"
                               "}"); // 设置样式
}

void DynamicPage::initCustomPlotStyle(QCustomPlot *customPlot)
{
    // 进行 QCustomPlot 样式初始化，你可以根据需要添加代码
    customPlot->xAxis->setLabel("x");                         // 设置x轴名称
    customPlot->yAxis->setLabel("y");                         // 设置y轴名称
    customPlot->xAxis->setLabelColor(QColor(226, 60, 255));   // 设置x轴名称颜色
    customPlot->yAxis->setLabelColor(QColor(226, 60, 255));   // 设置y轴名称颜色
    customPlot->xAxis->setTickLabelColor(Qt::yellow);         // 设置x轴坐标颜色
    customPlot->yAxis->setTickLabelColor(Qt::yellow);         // 设置y轴坐标颜色
    customPlot->xAxis->setBasePen(QPen(QColor(0, 0, 0)));     // 设置x轴坐标轴颜色
    customPlot->yAxis->setBasePen(QPen(QColor(25, 150, 92))); // 设置y轴坐标轴颜色
    // 设置画布背景色
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);

    // 显示图例
    customPlot->legend->setVisible(true);
    // 设置图例的位置
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    // 设置legend的字体大小
    customPlot->legend->setFont(QFont("Consolas", 14));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    customPlot->axisRect()->setRangeZoomAxes(customPlot->xAxis, nullptr);
    customPlot->axisRect()->setRangeZoomFactor(0.8);
    customPlot->replot(); // 重绘 每次改变完以后都要调用这个进行重新绘制
}

void DynamicPage::createPage(int index)
{
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    QCustomPlot *plot = new QCustomPlot();
    plots->append(plot);
    layout->addWidget(plot);
    initCustomPlotStyle(plot);
    // 如果有必要，这里可以添加更多的组件到页面

    stackedWidget->addWidget(page);
}

void DynamicPage::displayCurve(int pageIndex,
                               QVector<QVector<double> *> *xData,
                               QVector<QVector<double> *> *yData,
                               QString title,
                               QStringList legendList,
                               QStringList labelList)
{
    QCustomPlot *plot;
    if (pageIndex >= 0 && pageIndex < plots->size())
    {
        if (xData->size() == 1 && yData->size() >= 1)
        {
            plot = (*plots)[pageIndex];

            plot->clearGraphs();

            for (int i = 0; i < yData->size(); ++i)
            {
                plot->addGraph();
                plot->graph(i)->setPen(QPen(colorContainer.at(i), 3));
                plot->graph(i)->setData(*(*xData)[0], *(*yData)[i]);
                if (legendList != QStringList() && i < legendList.size())
                {
                    plot->graph(i)->setName(legendList.at(i));
                    // 删除取出的legendList中的元素
                    // legendList.removeAt(i);
                }
                emit storeRuntimeDataSignal(plot->graph(i)->data(),
                                            this->pageObjectId,
                                            this->getCurveNum(pageIndex) + i);
                delete (*yData)[i];
            }
            delete (*xData)[0];

            plot->rescaleAxes();
            plot->replot();

            // 检查是否已存在标题元素，并移除
            if (plot->plotLayout()->elementCount() > 0 && dynamic_cast<QCPTextElement *>(plot->plotLayout()->element(0, 0)))
            {
                plot->plotLayout()->removeAt(0);
                plot->plotLayout()->simplify(); // 清理空行
            }

            plot->plotLayout()->insertRow(0);
            plot->plotLayout()
                ->addElement(0, 0, new QCPTextElement(plot, title, QFont("sans", 12, QFont::Bold)));
            if (labelList.size() == 2)
            {
                plot->xAxis->setLabel(labelList.at(0));
                plot->yAxis->setLabel(labelList.at(1));
            }

            plot->xAxis->setLabelColor(QColor(226, 60, 255));
            plot->yAxis->setLabelColor(QColor(226, 60, 255));
            plot->xAxis->setLabelFont(QFont("Consolas", 12));
            plot->yAxis->setLabelFont(QFont("Consolas", 12));
            plot->xAxis->setLabelPadding(10);
            plot->yAxis->setLabelPadding(10);
        }
        else if (xData->size() > 1 && yData->size() == 1)
        {
            plot = (*plots)[pageIndex];
            plot->clearGraphs();

            for (int i = 0; i < xData->size(); ++i)
            {
                plot->addGraph();
                plot->graph(i)->setPen(QPen(colorContainer.at(i), 3));
                qDebug() << "xData->at(i)->size():" << xData->at(i)->size();
                qDebug() << "yData->at(0)->size():" << yData->at(0)->size();
                plot->graph(i)->setData(*(*xData)[i], *(*yData)[0]);
                if (legendList != QStringList() && i < legendList.size())
                {
                    qDebug() << "legendList.at(i):" << legendList.at(i);
                    plot->graph(i)->setName(legendList.at(i));
                }
                // emit storeRuntimeDataSignal(plot->graph(i)->data(), pageObjectId, getCurveNum(pageIndex) + i);
                delete (*xData)[i];
            }
            delete (*yData)[0];
            plot->rescaleAxes();
            plot->replot();

            // 检查是否已存在标题元素，并移除
            if (plot->plotLayout()->elementCount() > 0 && dynamic_cast<QCPTextElement *>(plot->plotLayout()->element(0, 0)))
            {
                plot->plotLayout()->removeAt(0);
                plot->plotLayout()->simplify(); // 清理空行
            }

            plot->plotLayout()->insertRow(0);
            plot->plotLayout()
                ->addElement(0, 0, new QCPTextElement(plot, title, QFont("sans", 12, QFont::Bold)));

            // 添加x轴名称和y轴名称
            if (labelList.size() == 2)
            {
                plot->xAxis->setLabel(labelList.at(0));
                plot->yAxis->setLabel(labelList.at(1));
            }
            // 设置x轴和y轴的名称的颜色和字体以及方向
            plot->xAxis->setLabelColor(QColor(226, 60, 255));
            plot->yAxis->setLabelColor(QColor(226, 60, 255));
            plot->xAxis->setLabelFont(QFont("Consolas", 12));
            plot->yAxis->setLabelFont(QFont("Consolas", 12));
            plot->xAxis->setLabelPadding(10);
            plot->yAxis->setLabelPadding(10);
        }

        delete xData;
        delete yData;
    }
}

void DynamicPage::updateDynamicView(QVector<QVector<double> *> *xData,
                                    QVector<QVector<double> *> *yData,
                                    int index,
                                    QString title,
                                    QStringList legendList,
                                    QStringList labelList)
{
    displayCurve(index, xData, yData, title, legendList, labelList);
}

QCustomPlot *DynamicPage::getCustomPlot(int index)
{

    return plots->at(index);
}

void DynamicPage::onPrevPageClicked()
{
    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex > 0)
    {
        stackedWidget->setCurrentIndex(currentIndex - 1);
    }
}

void DynamicPage::onNextPageClicked()
{
    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex < stackedWidget->count() - 1)
    {
        stackedWidget->setCurrentIndex(currentIndex + 1);
    }
}

void DynamicPage::onShowCursorClicked()
{
    // 这里可以添加显示光标的逻辑
    // 例如，可以使用QCustomPlot的功能来添加光标（crosshair）或者追踪点的标记
    // 注意：这需要自定义实现，QCustomPlot本身不直接支持光标功能
    qDebug() << "显示光标功能尚未实现";
}

void DynamicPage::onCloseClicked()
{
    this->close(); // 直接关闭当前窗口
}

int DynamicPage::getCurveNum(int pageIndex)
{
    // 获取plots容器中的所有plot的graph数量总和
    int totalGraphCount = 0;
    for (int i = 0; i < pageIndex; ++i)
    {
        totalGraphCount += (*plots)[i]->graphCount();
    }
    return totalGraphCount;
}
