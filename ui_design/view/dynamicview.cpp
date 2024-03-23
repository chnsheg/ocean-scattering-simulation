#include "dynamicview.h"

DynamicPage::DynamicPage(int pageCount, QWidget *parent)
    : QWidget(parent), pageCount(pageCount)
{
    setupUi();
    setAttribute(Qt::WA_DeleteOnClose);
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

void DynamicPage::createPage(int index)
{
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    QCustomPlot *plot = new QCustomPlot();
    plots.append(plot);
    layout->addWidget(plot);

    // 如果有必要，这里可以添加更多的组件到页面

    stackedWidget->addWidget(page);
}

void DynamicPage::displayCurve(int pageIndex,
                               QVector<QVector<double> *> *xData,
                               QVector<QVector<double> *> *yData,
                               const QString &title)
{
    if (pageIndex >= 0 && pageIndex < plots.size())
    {
        QCustomPlot *plot = plots[pageIndex];
        plot->clearGraphs();

        for (int i = 0; i < yData->size(); ++i)
        {
            plot->addGraph();
            plot->graph(i)->setData(*(*xData)[0], *(*yData)[i]);
            delete (*yData)[i];
        }
        delete (*xData)[0];
        delete xData;
        delete yData;

        plot->rescaleAxes();
        plot->replot();

        plot->plotLayout()->insertRow(0);
        plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, title, QFont("sans", 12, QFont::Bold)));
    }
}

void DynamicPage::updateDynamicView(QVector<QVector<double> *> *xData, QVector<QVector<double> *> *yData, int index)
{
    displayCurve(index, xData, yData, QString("Page %1").arg(index + 1));
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
