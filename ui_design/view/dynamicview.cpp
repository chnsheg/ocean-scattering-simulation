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

void DynamicPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);

    for (int i = 0; i < pageCount; ++i)
    {
        createPage(i);
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    prevPageButton = new QPushButton("上一面", this);
    nextPageButton = new QPushButton("下一面", this);
    showCursorButton = new QPushButton("显示光标", this);
    closeButton = new QPushButton("关闭", this);

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
                               QStringList legendList)
{
    QCustomPlot *plot;
    if (pageIndex >= 0 && pageIndex < plots->size()) {
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
                }
                emit storeRuntimeDataSignal(plot->graph(i)->data(),
                                            this->pageObjectId,
                                            this->getCurveNum(pageIndex) + i);
                delete (*yData)[i];
            }
            delete (*xData)[0];

            plot->rescaleAxes();
            plot->replot();

            plot->plotLayout()->insertRow(0);
            plot->plotLayout()
                ->addElement(0, 0, new QCPTextElement(plot, title, QFont("sans", 12, QFont::Bold)));
        }
        else if (xData->size() > 1 && yData->size() == 1)
        {
            plot = (*plots)[pageIndex];
            plot->clearGraphs();

            for (int i = 0; i < xData->size(); ++i)
            {
                plot->addGraph();
                plot->graph(i)->setPen(QPen(colorContainer.at(i), 3));
                plot->graph(i)->setData(*(*xData)[i], *(*yData)[0]);
                if (legendList != QStringList() && i < legendList.size())
                {
                    plot->graph(i)->setName(legendList.at(i));
                }
                // emit storeRuntimeDataSignal(plot->graph(i)->data(), pageObjectId, getCurveNum(pageIndex) + i);
                delete (*xData)[i];
            }
            delete (*yData)[0];
            plot->rescaleAxes();
            plot->replot();

            plot->plotLayout()->insertRow(0);
            plot->plotLayout()
                ->addElement(0, 0, new QCPTextElement(plot, title, QFont("sans", 12, QFont::Bold)));
        }

        delete xData;
        delete yData;
    }
}

void DynamicPage::updateDynamicView(QVector<QVector<double> *> *xData,
                                    QVector<QVector<double> *> *yData,
                                    int index,
                                    QString title,
                                    QStringList legendList)
{
    displayCurve(index, xData, yData, title, legendList);
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
