#include "retrievalwindow.h"
#include "ui_retrievalwindow.h"
#include "model/constantmap.h"
#include "model/constantstorage.h"
#include "model/pagedatagenerator.h"

RetrievalWindow::RetrievalWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::RetrievalWindow)
{
    ui->setupUi(this);
    // 绑定按键事件
    boundButtonEvent();
    // 存入lineEdit数据到内存中
    saveLineEditGroupsText();
    // PageDataGenerator *model = Singleton<PageDataGenerator>::getInstance(nullptr);
    // connect(model, &PageDataGenerator::retrievalCompleted, this, &RetrievalWindow::onRetrievalCompleted);

    for (auto &&customPlot : ui->centralwidget->findChildren<QCustomPlot *>())
    {
        Singleton<CustomPlotManager>::getInstance()->setCustomPlot(customPlot); // Tracer可能会出问题，只会锚定到最后一个customPlot上
    }
    Singleton<TextEditManager>::getInstance()->setTextEdit(getRetrievalTextEdit());
    Singleton<TextEditManager>::getInstance()->initTextEditStyle();

    // 初始化窗口样式
    initWindowStyle();
}

// setStyleSheet("QPushButton{"
//                                   "position: absolute;"
//                                   "top: 50%;"
//                                   "left: 50%;"
//                                   "font: 28px \"微软雅黑\";"
//                                   "font-weight: 500;"
//                                   "color: #FFFFFF;"
//                                   "width: 150px;"
//                                   "height: 60px;"
//                                   "background-color: rgba(52, 73, 94, 1);"
//                                   "border-color: #2f3640;"
//                                   "border-radius: 10px;"
//                                   "border-style: solid;"
//                                   "border-width: 2px;"
//                                   "margin-left: auto;"
//                                   "margin-top: 10px;"
//                                   "}"
//                                   "QPushButton:hover{"
//                                   "color: rgba(52, 73, 94, 1);"
//                                   "background-color: #FFFFFF;"
//                                   "border-color: #2f3640;"
//                                   "}"
//                                   "QPushButton::pressed,QPushButton:checked{"
//                                   "color: #FFFFFF;"
//                                   "background-color: qlineargradient(spread pad, x1 0, y1 0, x2 0, y2 1, stop 0 #273c75, stop 1 #487eb0);"
//                                   "}"
//                                   "QPushButton:disabled{"
//                                   "color: #2f3640;"
//                                   "background-color: #dcdde1;"
//                                   "border-color: #dcdde1;"
//                                   "}"); // 设置样式

void RetrievalWindow::initWindowStyle()
{
    // 查找ui中的所有按键
    for (auto &&button : ui->centralwidget->findChildren<QPushButton *>())
    {
        button->setStyleSheet("QPushButton{"
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

    QCustomPlot *customPlot1 = getRetrievalCustomPlot();
    QCustomPlot *customPlot2 = getMesurementCustomPlot();

    // 设置customPlot1按照y轴放大缩小
    customPlot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot1->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
    customPlot1->axisRect()->setRangeZoom(Qt::Vertical);
    customPlot1->axisRect()->setRangeZoomAxes(nullptr, customPlot1->yAxis);
    customPlot1->axisRect()->setRangeZoomFactor(0.5, 0.5);

    // 设置customPlot2按照y轴放大缩小
    customPlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot2->axisRect()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
    customPlot2->axisRect()->setRangeZoom(Qt::Vertical);
    customPlot2->axisRect()->setRangeZoomAxes(nullptr, customPlot2->yAxis);
    customPlot2->axisRect()->setRangeZoomFactor(0.5, 0.5);

    // 初始化进度条控件
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat("PMT反演进度：%p%");
    ui->progressBar->setAlignment(Qt::AlignCenter);
    // 美化进度条样式
    ui->progressBar->setStyleSheet("QProgressBar{"
                                   "border: 2px solid grey;"
                                   "border-radius: 5px;"
                                   "text-align: center;"
                                   "}"
                                   "QProgressBar::chunk{"
                                   "background-color: #487eb0;"
                                   "width: 10px;"
                                   "}"); // 设置样式
}

RetrievalWindow::~RetrievalWindow()
{
    this->close();
    delete ui;
}

// 绑定按键事件函数
void RetrievalWindow::boundButtonEvent()
{
    // 绑定“显示”按钮点击事件
    connect(ui->show3_2, &QPushButton::clicked, this, &RetrievalWindow::onShowButtonClicked);
    // 绑定“重置”按钮点击事件
    connect(ui->clear3_2, &QPushButton::clicked, this, &RetrievalWindow::onClearButtonClicked);

    connect(ui->tracer3_2, &QPushButton::clicked, this, &RetrievalWindow::onExtendButtonClicked);

    // 绑定“统计”按钮点击事件
    connect(ui->stat3_2, &QPushButton::clicked, this, &RetrievalWindow::onStatisticButtonClicked);
}

// “显示”按钮点击事件
void RetrievalWindow::onShowButtonClicked()
{
    saveLineEditGroupsText();

    PageDataGenerator *model = Singleton<PageDataGenerator>::getInstance(nullptr);
    // 绑定反演结束信号
    connect(model, &PageDataGenerator::retrievalCompleted, this, &RetrievalWindow::onRetrievalCompleted);
    model->generateDynamicAction(0);
}

void RetrievalWindow::onExtendButtonClicked()
{
    saveLineEditGroupsText();
    // 清除原来的数据
    m_mesurementError.clear();
    m_retrievalError.clear();

    PageDataGenerator *model = Singleton<PageDataGenerator>::getInstance(nullptr);
    // 绑定反演结束信号
    connect(model, &PageDataGenerator::retrievalCompleted, this, &RetrievalWindow::onRetrievalCompleted);
    model->caculateRetrievalErrorByDepth();
}

// 对m_retrievalError中的数据进行直方图统计
void RetrievalWindow::onStatisticButtonClicked()
{
    DynamicPage *dynamicView;
    int anchor = 0;

    if (dynamicViewOpened.contains(0)) // 统计按键对应的动态页面索引为0
    {
        // 查找index对应的dynamicViewOpened中的索引
        anchor = dynamicViewOpened.indexOf(0);
        dynamicView = dynamicViewVector.at(anchor);
    }
    else
    {
        dynamicView = new DynamicPage(2);
        dynamicViewVector.append(dynamicView);
        dynamicViewOpened.append(0);
        anchor = dynamicViewVector.size() - 1; // 指向当前打开的dynamicView
    }

    connect(dynamicView, &DynamicPage::closeDynamicPageSignal, this, [=](int index)
            {
        dynamicViewVector.removeOne(dynamicView);
        dynamicViewOpened.removeOne(anchor); });

    calculateTSStatistics(dynamicViewVector.at(anchor));
}

// 对m_retrievalError中的数据计算直方图统计
void RetrievalWindow::calculateTSStatistics(DynamicPage *dynamicView)
{
    // 首先判断m_retrievalError中是否有数据
    if (m_retrievalError.size() == 0)
    {
        Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "无法进行直方图统计，请先生成数据", Logger::Warning);
        return;
    }

    // 计算温度和盐度误差的直方图统计
    QVector<double> temperatureError;
    QVector<double> salinityError;
    for (auto &&retrievalError : m_retrievalError)
    {
        temperatureError.push_back(std::abs(retrievalError.temperatureError));
        salinityError.push_back(std::abs(retrievalError.salinityError));
    }
    int num1 = temperatureError.size() / 2 <= 20 ? temperatureError.size() / 2 : 20; // 点数越多，区间越窄
    int num2 = salinityError.size() / 2 <= 20 ? salinityError.size() / 2 : 20;

    if (num1 != 20)
    {
        num1 = temperatureError.size() / 2 >= 10 ? num1 : 10;
    }

    if (num2 != 20)
    {
        num2 = salinityError.size() / 2 >= 10 ? num2 : 10;
    }

    double maxValue1 = *std::max_element(temperatureError.begin(), temperatureError.end());
    double minValue1 = *std::min_element(temperatureError.begin(), temperatureError.end());
    double maxValue2 = *std::max_element(salinityError.begin(), salinityError.end());
    double minValue2 = *std::min_element(salinityError.begin(), salinityError.end());

    double m_maxRange1 = maxValue1 + 0.5 * (maxValue1 - minValue1);
    double m_minRange1 = minValue1 - 0.5 * (maxValue1 - minValue1);
    double m_maxRange2 = maxValue2 + 0.5 * (maxValue2 - minValue2);
    double m_minRange2 = minValue2 - 0.5 * (maxValue2 - minValue2);

    drawHistogram(dynamicView->getCustomPlot(0), calculateHistogram(temperatureError, num1), "Temperature Error");
    drawHistogram(dynamicView->getCustomPlot(1), calculateHistogram(salinityError, num2), "Salinity Error");

    // 设置坐标轴范围
    dynamicView->getCustomPlot(0)->rescaleAxes(true);
    dynamicView->getCustomPlot(1)->rescaleAxes(true);

    dynamicView->getCustomPlot(0)->replot();
    dynamicView->getCustomPlot(1)->replot();
    dynamicView->show();

    void (QCPAxis::*rangeChanged)(const QCPRange &, const QCPRange &) = &QCPAxis::rangeChanged;
    // 绑定customPlot的坐标轴范围变化信号到匿名函数
    connect(dynamicView->getCustomPlot(0)->xAxis, rangeChanged, this, [=](const QCPRange &newRange, const QCPRange &oldRange)
            {
            // 如果新范围相比旧范围变大或缩小2倍，则重新计算直方图统计
            double newRangeSize = newRange.upper - newRange.lower;
            // double oldRangeSize = m_maxRange - m_minRange;
            double oldRangeSize = m_maxRange1 - m_minRange1;
            if (newRangeSize > oldRangeSize * 1.5 || newRangeSize < oldRangeSize / 1.5)
            {
            // m_maxRange = newRange.upper;
            // m_minRange = newRange.lower;
            double scale = oldRangeSize/newRangeSize;
                // 首先判断m_retrievalError中是否有数据
            if (m_retrievalError.size() == 0)
            {
                Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "无法进行直方图统计，请先生成数据", Logger::Warning);
                return;
            }

            // 计算温度和盐度误差的直方图统计
            QVector<double> temperatureError;
            for (auto &&retrievalError : m_retrievalError)
            {
                temperatureError.push_back(std::abs(retrievalError.temperatureError));
            }
            // int num1 = temperatureError.size() / 2 >=10 ? temperatureError.size() / 2 : 10;

            int num1 = temperatureError.size() / 2 <= 20 ? temperatureError.size() / 2 : 20; // 点数越多，区间越窄

            if (num1 != 20)
            {
                num1 = temperatureError.size() / 2 >= 10 ? num1 : 10;
            }
            
                drawHistogram(dynamicView->getCustomPlot(0), calculateHistogramWithinRange(temperatureError, newRange.lower, newRange.upper, (int)(num1 *scale)), "Temperature Error");
                // 缩放customPlot的x轴范围
                dynamicView->getCustomPlot(0)->xAxis->setRange(newRange);
                dynamicView->getCustomPlot(0)->replot();
            } });

    connect(dynamicView->getCustomPlot(1)->xAxis, rangeChanged, this, [=](const QCPRange &newRange, const QCPRange &oldRange)
            {
            // 如果新范围相比旧范围变大或缩小2倍，则重新计算直方图统计
            double newRangeSize = newRange.upper - newRange.lower;
            // double oldRangeSize = m_maxRange - m_minRange;
            double oldRangeSize = m_maxRange2 - m_minRange2;
            if (newRangeSize > oldRangeSize * 1.5 || newRangeSize < oldRangeSize / 1.5){
            // m_maxRange = newRange.upper;
            // m_minRange = newRange.lower;
            double scale = oldRangeSize/newRangeSize;
                // 首先判断m_retrievalError中是否有数据
            if (m_retrievalError.size() == 0)
            {
                Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "无法进行直方图统计，请先生成数据", Logger::Warning);
                return;
            }

            // 计算温度和盐度误差的直方图统计
            QVector<double> salinityError;
            for (auto &&retrievalError : m_retrievalError)
            {
                salinityError.push_back(std::abs(retrievalError.salinityError));
            }
            // int num2 = salinityError.size() / 2 >=10 ? salinityError.size() / 2 : 10;
            int num2 = salinityError.size() / 2 <= 20 ? salinityError.size() / 2 : 20;


            if (num2 != 20)
            {
                num2 = salinityError.size() / 2 >= 10 ? num2 : 10;
            }
                drawHistogram(dynamicView->getCustomPlot(1), calculateHistogramWithinRange(salinityError, newRange.lower, newRange.upper, (int)(num2 *scale)), "Salinity Error");
                // 缩放customPlot的x轴范围
                dynamicView->getCustomPlot(1)->xAxis->setRange(newRange);
                dynamicView->getCustomPlot(1)->replot();
            } });
}

/**
 * @brief 计算直方图统计
 * @param data 数据
 * @param binCount 区间个数
 * @return QMap<double, int> 直方图统计结果，key为区间中心，value为区间内数据个数
 */
QMap<double, int> RetrievalWindow::calculateHistogram(const QVector<double> &data, int binCount)
{
    QMap<double, int> histogram;
    if (data.isEmpty())
    {
        return histogram;
    }

    // 计算数据的最小值和最大值
    double minValue = *std::min_element(data.begin(), data.end());
    double maxValue = *std::max_element(data.begin(), data.end());

    // 计算每个区间的宽度
    double binWidth = (maxValue - minValue) / binCount;

    // m_maxRange = maxValue + 0.5 * binWidth;
    // m_minRange = minValue - 0.5 * binWidth;

    // 初始化直方图的每个区间
    for (int i = 0; i < binCount; ++i)
    {
        double binCenter = minValue + (i + 0.5) * binWidth;
        histogram[binCenter] = 0;
    }

    // 对数据进行归类
    for (double value : data)
    {
        int binIndex = std::floor((value - minValue) / binWidth);
        if (binIndex >= binCount)
        {
            binIndex = binCount - 1; // 把最大值放入最后一个区间
        }
        double binCenter = minValue + (binIndex + 0.5) * binWidth;
        histogram[binCenter]++;
    }

    return histogram;
}

QMap<double, int> RetrievalWindow::calculateHistogramWithinRange(const QVector<double> &data, double minRange, double maxRange, int binCount)
{
    QMap<double, int> histogram;
    if (data.isEmpty())
    {
        return histogram;
    }

    // 过滤出在指定范围内的数据
    // QVector<double> filteredData;
    // for (double value : data)
    // {
    //     if (value >= minRange && value <= maxRange)
    //     {
    //         filteredData.append(value);
    //     }
    // }

    if (data.isEmpty())
    {
        return histogram;
    }

    // 计算过滤后数据的最小值和最大值
    double minValue = *std::min_element(data.begin(), data.end());
    double maxValue = *std::max_element(data.begin(), data.end());

    // 计算每个区间的宽度
    double binWidth = (maxValue - minValue) / binCount;

    // 初始化直方图的每个区间
    for (int i = 0; i < binCount; ++i)
    {
        double binCenter = minValue + (i + 0.5) * binWidth;
        histogram[binCenter] = 0;
    }

    // 对过滤后的数据进行归类
    for (double value : data)
    {
        int binIndex = std::floor((value - minValue) / binWidth);
        if (binIndex >= binCount)
        {
            binIndex = binCount - 1; // 把最大值放入最后一个区间
        }
        double binCenter = minValue + (binIndex + 0.5) * binWidth;
        histogram[binCenter]++;
    }

    // 仅截取前40个histogram显示
    if (histogram.size() > 80)
    {
        QMap<double, int> newHistogram;
        int i = 0;
        for (auto it = histogram.begin(); it != histogram.end(); ++it)
        {
            if (i < 80)
            {
                newHistogram[it.key()] = it.value();
            }
            else
            {
                break;
            }
            i++;
        }
        return newHistogram;
    }

    return histogram;
}

void RetrievalWindow::drawHistogram(QCustomPlot *customPlot, const QMap<double, int> &histogram, QString title)
{
    QVector<double> binCenters;
    QVector<double> binHeights;

    // 遍历直方图数据，填充绘图用的 QVector
    for (auto it = histogram.begin(); it != histogram.end(); ++it)
    {
        binCenters.append(it.key());
        binHeights.append(it.value());
    }

    QCPBars *bars = nullptr;

    for (int i = 0; i < customPlot->plottableCount(); ++i)
    {
        // if (qobject_cast<QCPBars *>(customPlot->plottable(i)))
        // {
        //     bars = qobject_cast<QCPBars *>(customPlot->plottable(i));
        //     // 这里假设customPlot只能有一个柱形图，如果有多个柱形图，可以在这里继续循环或者退出循环
        //     break;
        // }

        if (customPlot->plottable(i)->objectName() == "Bars")
        {
            bars = qobject_cast<QCPBars *>(customPlot->plottable(i));
            // 这里假设customPlot只能有一个柱形图，如果有多个柱形图，可以在这里继续循环或者退出循环
            break;
        }
    }

    // 创建柱状图
    if (!bars)
    {
        bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
        bars->setObjectName("Bars");
    }

    // 设置柱状图颜色
    bars->setPen(QPen(QColor(0, 160, 140).lighter(130))); // 设置柱状图的边框颜色
    // bars->setBrush(QColor(20, 68, 106));                  // 设置柱状图的画刷颜色
    // 设置透明度
    bars->setBrush(QColor(20, 68, 106, 128));

    // 设置柱状图的宽度
    if (binCenters.size() > 1)
    {
        bars->setWidth(binCenters[1] - binCenters[0]);
    }
    else
    {
        bars->setWidth(1); // 当只有一个数据点时，设置宽度为1
    }

    bars->setData(binCenters, binHeights);

    // 设置坐标轴的标签
    customPlot->xAxis->setLabel("Value");
    customPlot->yAxis->setLabel("Frequency");

    // 使用 QCPAxisTickerText 设置自定义的刻度标签
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    for (double center : binCenters)
    {
        textTicker->addTick(center, QString::number(center, 'f', 2)); // 添加刻度和标签
    }
    customPlot->xAxis->setTicker(textTicker);

    // 如果原来直方图顶部存在文本标签，则拿到QCustomPlot中的QCPItemText
    QCPItemText *textLabel = nullptr;
    QCPItemText *widthLabel = nullptr;

    // 如果原来界面上有valueLabels，先删除
    for (int i = 0; i < customPlot->layerCount(); ++i)
    {
        if (customPlot->layer(i)->name() == "valueLabels")
        {
            // 删除layer(i)层中的QCPItemText
            for (int j = 0; j < customPlot->layer(i)->children().size(); ++j)
            {
                // 清除这个图层中的元素
                customPlot->layer(i)->children().at(j)->setVisible(false);
            }
        }
        else if (customPlot->layer(i)->name() == "widthLabel")
        {
            // 删除layer(i)层中的QCPItemText
            for (int j = 0; j < customPlot->layer(i)->children().size(); ++j)
            {
                // 清除这个图层中的元素
                customPlot->layer(i)->children().at(j)->setVisible(false);
            }
        }
    }

    // 添加直方图顶部的文本标签（显示频数）
    for (int i = 0; i < binCenters.size(); ++i)
    {
        customPlot->addLayer("valueLabels", nullptr, QCustomPlot::limAbove); // 添加一个图层
        textLabel = new QCPItemText(customPlot);
        textLabel->setPositionAlignment(Qt::AlignBottom | Qt::AlignHCenter);
        textLabel->position->setType(QCPItemPosition::ptPlotCoords);
        textLabel->position->setCoords(binCenters[i], binHeights[i]); // 设置文本标签的位置
        textLabel->setText(QString::number(binHeights[i]));           // 显示的文本内容
        textLabel->setFont(QFont("Helvetica", 12));                   // 设置字体和大小
        textLabel->setPen(QPen(Qt::NoPen));                           // 设置文本边框颜色
        textLabel->setBrush(QBrush(QColor(128, 128, 0, 0)));          // 设置文本背景颜色
        textLabel->setColor(QColor(255, 255, 255));                   // 设置文本颜色
        textLabel->setLayer("valueLabels");                           // 设置图层
    }

    // 检查是否已存在标题元素，并移除
    if (customPlot->plotLayout()->elementCount() > 0 && dynamic_cast<QCPTextElement *>(customPlot->plotLayout()->element(0, 0)))
    {
        customPlot->plotLayout()->removeAt(0);
        customPlot->plotLayout()->simplify(); // 清理空行
    }

    // 显示柱状图的宽度
    customPlot->addLayer("widthLabel", nullptr, QCustomPlot::limAbove);
    widthLabel = new QCPItemText(customPlot);
    widthLabel->setPositionAlignment(Qt::AlignTop | Qt::AlignRight);
    widthLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    // 显示到正中间
    widthLabel->position->setCoords(0.9, 0.05);
    if (binCenters.size() > 1)
    {
        widthLabel->setText(QString("Bar Width: %1").arg(binCenters[1] - binCenters[0], 0, 'f', 3));
    }
    else
    {
        widthLabel->setText(QString("Bar Width: 0"));
    }
    // widthLabel->setText(QString("Bar Width: %1").arg(binCenters[1] - binCenters[0], 0, 'f', 3));
    widthLabel->setFont(QFont("Helvetica", 12, QFont::Bold));
    widthLabel->setPen(QPen(QColor(255, 255, 255, 0)));
    widthLabel->setBrush(QBrush(QColor(255, 255, 255, 0)));
    widthLabel->setColor(QColor(255, 255, 255));
    widthLabel->setLayer("widthLabel");

    // 设置标题
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(customPlot, title, QFont("sans", 14, QFont::Bold)));

    // 自动缩放坐标轴以适应数据
    // customPlot->rescaleAxes();

    // customPlot->replot();
}

// 反演完毕后的事件处理
void RetrievalWindow::onRetrievalCompleted(QVariantList *args)
{
    PageDataGenerator *model = Singleton<PageDataGenerator>::getInstance(nullptr);
    // 绑定反演结束信号
    disconnect(model, &PageDataGenerator::retrievalCompleted, this, &RetrievalWindow::onRetrievalCompleted);
    if (args->at(0).toInt() == 1)
    {
        Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "PMT反演成功！", Logger::Info);
        // 绘制散点图
        // drawRetrievalErrorScatterPlot();
        calculateRetrievalError();
    }
    else if (args->at(0).toInt() == 0)
    {
        Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "PMT反演失败！", Logger::Warning);
    }
    else if (args->at(0).toInt() == 2)
    {
        Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "开始进行各深度下的PMT数据反演！", Logger::Info);
        calculateDepthsRetrievalError(args->at(1).toInt(), args->at(2).toDouble(), args->at(3).value<QVector<double> *>());
    }
}

void RetrievalWindow::drawRetrievalErrorScatterPlot()
{
    // 绘制散点图
    QCustomPlot *customPlot = getRetrievalCustomPlot();
    if (customPlot)
    {
        // 将m_retrievalError中所有的RetrievalError数据中的temperatureError和salinityError提取出来
        QVector<double> temperatureError;
        QVector<double> salinityError;
        QVector<double> retrievalTimes;
        double max_y = std::abs(m_retrievalError.value(0).temperatureError);
        double max_x = 0;
        for (auto &&retrievalError : m_retrievalError)
        {
            temperatureError.push_back(retrievalError.temperatureError);
            salinityError.push_back(retrievalError.salinityError);
            // retrievalTimes.push_back(m_retrievalError.key(retrievalError));
            for (auto it = m_retrievalError.constBegin(); it != m_retrievalError.constEnd(); ++it)
            {
                if (it.value() == retrievalError && retrievalTimes.contains(it.key()) == false)
                {
                    retrievalTimes.push_back(it.key());
                }
            }

            qDebug() << "RetrievalError: " << retrievalError.temperatureError << " " << retrievalError.salinityError;

            // 顺便查找在所有数据中的绝对值的最大值
            if (std::abs(retrievalError.temperatureError) > max_y)
            {
                max_y = std::abs(retrievalError.temperatureError);
            }
            if (std::abs(retrievalError.salinityError) > max_y)
            {
                max_y = std::abs(retrievalError.salinityError);
            }
            if (retrievalTimes.back() > max_x)
            {
                max_x = retrievalTimes.back();
            }
        }

        // 清空原来的散点图
        customPlot->clearGraphs();

        // 创建散点图

        customPlot->addGraph();
        customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 16));
        customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
        customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 255)));
        customPlot->graph(0)->setData(retrievalTimes, temperatureError);
        customPlot->graph(0)->setPen(QPen(Qt::blue));
        customPlot->graph(0)->setName("Temperature Error");
        customPlot->xAxis->setLabel("Retrieval Times");
        customPlot->yAxis->setLabel("Error (°C/‰)");
        customPlot->xAxis->setRange(-0.1 * max_x, max_x * 1.1);
        if (max_y == 0)
        {
            max_y = 1;
        }
        else if (max_y < 0)
        {
            max_y = -max_y;
        }
        customPlot->yAxis->setRange(-1.2 * max_y, 1.2 * max_y);

        customPlot->addGraph();
        customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 16));
        customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
        customPlot->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 255)));
        customPlot->graph(1)->setData(retrievalTimes, salinityError);
        customPlot->graph(1)->setPen(QPen(Qt::red));
        customPlot->graph(1)->setName("Salinity Error");
        customPlot->replot();
    }
}

void RetrievalWindow::drawMesurementErrorScatterPlot()
{
    // 绘制散点图
    QCustomPlot *customPlot = getMesurementCustomPlot();
    if (customPlot)
    {
        // 将m_mesurementError中所有的MesurementError数据中的BrillouinShiftError、BrillouinWidthError和rayleighWidthError提取出来
        QVector<double> BrillouinShiftError;
        QVector<double> BrillouinWidthError;
        QVector<double> rayleighWidthError;
        QVector<double> retrievalTimes;
        double max_y = std::abs(m_mesurementError.value(0).BrillouinShiftError);
        double max_x = 0;

        for (auto &&mesurementError : m_mesurementError)
        {
            BrillouinShiftError.push_back(mesurementError.BrillouinShiftError);
            BrillouinWidthError.push_back(mesurementError.BrillouinWidthError);
            rayleighWidthError.push_back(mesurementError.rayleighWidthError);
            // retrievalTimes.push_back(m_mesurementError.key(mesurementError));

            for (auto it = m_mesurementError.constBegin(); it != m_mesurementError.constEnd(); ++it)
            {
                if (it.value() == mesurementError && retrievalTimes.contains(it.key()) == false)
                {
                    retrievalTimes.push_back(it.key());
                }
            }

            qDebug() << "retrievalTimes" << retrievalTimes.back();

            // 顺便查找在所有数据中的绝对值的最大值
            if (std::abs(mesurementError.BrillouinShiftError) > max_y)
            {
                max_y = std::abs(mesurementError.BrillouinShiftError);
            }
            if (std::abs(mesurementError.BrillouinWidthError) > max_y)
            {
                max_y = std::abs(mesurementError.BrillouinWidthError);
            }
            if (std::abs(mesurementError.rayleighWidthError) > max_y)
            {
                max_y = std::abs(mesurementError.rayleighWidthError);
            }
            if (retrievalTimes.back() > max_x)
            {
                max_x = retrievalTimes.back();
            }
        }

        // 清空原来的散点图
        customPlot->clearGraphs();

        // 创建散点图
        customPlot->addGraph();
        customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 16));
        customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
        customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 255)));
        customPlot->graph(0)->setData(retrievalTimes, BrillouinShiftError);
        customPlot->graph(0)->setPen(QPen(Qt::blue));
        customPlot->graph(0)->setName("Brillouin Shift Error");
        customPlot->xAxis->setLabel("Retrieval Times");
        customPlot->yAxis->setLabel("Error (GHz)");
        // customPlot->xAxis->setRange(0, retrievalTimes.size() + 1);
        customPlot->xAxis->setRange(-0.1 * max_x, max_x * 1.1);
        if (max_y == 0)
        {
            max_y = 1;
        }
        else if (max_y < 0)
        {
            max_y = -max_y;
        }
        customPlot->yAxis->setRange(-1.2 * max_y, 1.2 * max_y);

        customPlot->addGraph();
        customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 16));
        customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
        customPlot->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 255)));
        customPlot->graph(1)->setData(retrievalTimes, BrillouinWidthError);
        customPlot->graph(1)->setPen(QPen(Qt::red));
        customPlot->graph(1)->setName("Brillouin Width Error");

        customPlot->addGraph();
        customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 16));
        customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
        customPlot->graph(2)->setBrush(QBrush(QColor(0, 0, 255, 255)));
        customPlot->graph(2)->setData(retrievalTimes, rayleighWidthError);
        customPlot->graph(2)->setPen(QPen(Qt::green));
        customPlot->graph(2)->setName("Rayleigh Width Error");
        customPlot->replot();
    }
}

// 计算反演误差
void RetrievalWindow::calculateRetrievalError()
{
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);

    // constantStorage->setConstant(constantMap->getConstantName(6, 7), QVariant::fromValue(res_B_shift));
    //     constantStorage->setConstant(constantMap->getConstantName(6, 8), QVariant::fromValue(res_B_width));
    //     constantStorage->setConstant(constantMap->getConstantName(6, 9), QVariant::fromValue(res_R_width));
    //     constantStorage->setConstant(constantMap->getConstantName(6, 10), QVariant::fromValue(REF_Tem));
    //     constantStorage->setConstant(constantMap->getConstantName(6, 11), QVariant::fromValue(REF_Sal));
    double res_B_shift = constantStorage->getConstant(constantMap->getConstantName(6, 7)).toDouble();
    double res_B_width = constantStorage->getConstant(constantMap->getConstantName(6, 8)).toDouble();
    double res_R_width = constantStorage->getConstant(constantMap->getConstantName(6, 9)).toDouble();
    double REF_Tem = constantStorage->getConstant(constantMap->getConstantName(6, 10)).toDouble();
    double REF_Sal = constantStorage->getConstant(constantMap->getConstantName(6, 11)).toDouble();
    double Bri_shift = constantStorage->getConstant(constantMap->getConstantName(6, 1)).toDouble() * 1e9;
    double Bri_width = constantStorage->getConstant(constantMap->getConstantName(6, 0)).toDouble() * 1e9;
    double Ray_Width = 0.15e9;
    double Water_Temperature = constantStorage->getConstant(constantMap->getConstantName(1, 0)).toDouble();
    double Water_Salinity = constantStorage->getConstant(constantMap->getConstantName(1, 1)).toDouble();

    // TODO
    double Error_shift = res_B_shift - Bri_shift;
    double Error_width = res_B_width - Bri_width;
    double Error_Rwidth = res_R_width - Ray_Width;
    // double Error_Photon = res_N_photo - PhotonNum;

    // Error.tem = REF.Tem - EnvironmentalFactors.tem;
    // Error.sal = REF.Sal - EnvironmentalFactors.sal;

    double Error_tem = REF_Tem - Water_Temperature;
    double Error_sal = REF_Sal - Water_Salinity;

    int fontSize = 8;

    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "Result", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "-------------------------------------------------------------------------", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "Spectral parameters|Measured(GHz)|Theoretical(GHz)|error(GHz)", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), " Brillouin width   " + QString::number(res_B_width / 1e9) + "    " + QString::number(Bri_width / 1e9) + "    " + QString::number(Error_width / 1e9), Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), " Brillouin shift   " + QString::number(res_B_shift / 1e9) + "  " + QString::number(Bri_shift / 1e9) + "  " + QString::number(Error_shift / 1e9), Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), " RAyleigh width    " + QString::number(res_R_width / 1e9) + "  " + QString::number(Ray_Width / 1e9) + "  " + QString::number(Error_Rwidth / 1e9), Logger::Info, fontSize);

    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "-------------------------------------------------------------------------", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "Parameters | Measured | Theoretical | error (%4.2f m: %4.2f)|", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "Environmental tem| " + QString::number(REF_Tem) + " °C | " + QString::number(Water_Temperature) + " °C | " + QString::number(Error_tem) + " °C |", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "Environmental Sal| " + QString::number(REF_Sal) + " ‰ | " + QString::number(Water_Salinity) + " ‰  | " + QString::number(Error_sal) + " ‰ |", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "-------------------------------------------------------------------------", Logger::Info, fontSize);

    // 封装误差数据
    MesurementError mesurementError;
    mesurementError.BrillouinShiftError = Error_shift / 1e9;
    mesurementError.BrillouinWidthError = Error_width / 1e9;
    mesurementError.rayleighWidthError = Error_Rwidth / 1e9;

    RetrievalError retrievalError;
    retrievalError.temperatureError = Error_tem;
    retrievalError.salinityError = Error_sal;

    // m_mesurementError.push_back(mesurementError);
    // m_retrievalError.push_back(retrievalError);
    double index = m_mesurementError.size();
    m_mesurementError.insert(index, mesurementError);
    m_retrievalError.insert(index, retrievalError);

    // 绘制散点图
    drawMesurementErrorScatterPlot();
    drawRetrievalErrorScatterPlot();
}

// 计算反演误差
void RetrievalWindow::calculateDepthsRetrievalError(int index, double depth, QVector<double> *retrievalData)
{
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);

    // constantStorage->setConstant(constantMap->getConstantName(6, 7), QVariant::fromValue(res_B_shift));
    //     constantStorage->setConstant(constantMap->getConstantName(6, 8), QVariant::fromValue(res_B_width));
    //     constantStorage->setConstant(constantMap->getConstantName(6, 9), QVariant::fromValue(res_R_width));
    //     constantStorage->setConstant(constantMap->getConstantName(6, 10), QVariant::fromValue(REF_Tem));
    //     constantStorage->setConstant(constantMap->getConstantName(6, 11), QVariant::fromValue(REF_Sal));
    // double res_B_shift = constantStorage->getConstant(constantMap->getConstantName(6, 7)).toDouble();
    // double res_B_width = constantStorage->getConstant(constantMap->getConstantName(6, 8)).toDouble();
    // double res_R_width = constantStorage->getConstant(constantMap->getConstantName(6, 9)).toDouble();
    // double REF_Tem = constantStorage->getConstant(constantMap->getConstantName(6, 10)).toDouble();
    // double REF_Sal = constantStorage->getConstant(constantMap->getConstantName(6, 11)).toDouble();
    QVector<double> *data = retrievalData;
    double res_B_shift = data->at(0);
    double res_B_width = data->at(1);
    double res_R_width = data->at(2);
    double REF_Tem = data->at(3);
    double REF_Sal = data->at(4);

    double Bri_shift = constantStorage->getConstant(constantMap->getConstantName(6, 1)).toDouble() * 1e9;
    double Bri_width = constantStorage->getConstant(constantMap->getConstantName(6, 0)).toDouble() * 1e9;
    double Ray_Width = 0.15e9;
    double Water_Temperature = constantStorage->getConstant(constantMap->getConstantName(1, 0)).toDouble();
    double Water_Salinity = constantStorage->getConstant(constantMap->getConstantName(1, 1)).toDouble();

    // TODO
    double Error_shift = res_B_shift - Bri_shift;
    double Error_width = res_B_width - Bri_width;
    double Error_Rwidth = res_R_width - Ray_Width;
    // double Error_Photon = res_N_photo - PhotonNum;

    // Error.tem = REF.Tem - EnvironmentalFactors.tem;
    // Error.sal = REF.Sal - EnvironmentalFactors.sal;

    double Error_tem = REF_Tem - Water_Temperature;
    double Error_sal = REF_Sal - Water_Salinity;

    int fontSize = 8;

    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "Result Index: " + QString::number(index), Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "-------------------------------------------------------------------------", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "Spectral parameters|Measured(GHz)|Theoretical(GHz)|error(GHz)", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), " Brillouin width   " + QString::number(res_B_width / 1e9) + "    " + QString::number(Bri_width / 1e9) + "    " + QString::number(Error_width / 1e9), Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), " Brillouin shift   " + QString::number(res_B_shift / 1e9) + "  " + QString::number(Bri_shift / 1e9) + "  " + QString::number(Error_shift / 1e9), Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), " RAyleigh width    " + QString::number(res_R_width / 1e9) + "  " + QString::number(Ray_Width / 1e9) + "  " + QString::number(Error_Rwidth / 1e9), Logger::Info, fontSize);

    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "-------------------------------------------------------------------------", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "Parameters | Measured | Theoretical | error (%4.2f m: %4.2f)|", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "Environmental tem| " + QString::number(REF_Tem) + " °C | " + QString::number(Water_Temperature) + " °C | " + QString::number(Error_tem) + " °C |", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "Environmental Sal| " + QString::number(REF_Sal) + " ‰ | " + QString::number(Water_Salinity) + " ‰  | " + QString::number(Error_sal) + " ‰ |", Logger::Info, fontSize);
    Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "-------------------------------------------------------------------------", Logger::Info, fontSize);

    // 封装误差数据
    MesurementError mesurementError;
    mesurementError.BrillouinShiftError = Error_shift / 1e9;
    mesurementError.BrillouinWidthError = Error_width / 1e9;
    mesurementError.rayleighWidthError = Error_Rwidth / 1e9;

    RetrievalError retrievalError;
    retrievalError.temperatureError = Error_tem;
    retrievalError.salinityError = Error_sal;

    qDebug() << "Error_depth: " << depth;

    m_mesurementError.insert(depth, mesurementError);
    m_retrievalError.insert(depth, retrievalError);

    // 绘制散点图
    drawMesurementErrorScatterPlot();
    drawRetrievalErrorScatterPlot();

    // constantStorage->setConstant(constantMap->getConstantName(9, 9), ui->lineEdit_1->text());
    if (m_mesurementError.size() == Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(constantMap->getConstantName(9, 9)).toInt())
    {
        Singleton<Logger>::getInstance()->logMessage(getRetrievalTextEdit(), "各深度下的PMT数据反演结束！", Logger::Info);
        PageDataGenerator *model = Singleton<PageDataGenerator>::getInstance(nullptr);
        disconnect(model, &PageDataGenerator::retrievalCompleted, this, &RetrievalWindow::onRetrievalCompleted);
        // 在温盐图中绘制一条T=0.5℃的等温线和S=1‰的等盐线
        QCustomPlot *customPlot = getRetrievalCustomPlot();
        customPlot->addGraph();
        customPlot->graph(2)->setPen(QPen(Qt::yellow, 2, Qt::DashLine));
        customPlot->graph(2)->setName("T=0.5℃");

        for (int i = 0; i < 80; i++)
        {
            customPlot->graph(2)->addData(i, 0.5);
        }

        customPlot->addGraph();
        customPlot->graph(3)->setPen(QPen(Qt::yellow, 2, Qt::DashLine));
        customPlot->graph(3)->setName("T=-0.5℃");

        for (int i = 0; i < 80; i++)
        {
            customPlot->graph(3)->addData(i, -0.5);
        }

        customPlot->addGraph();
        customPlot->graph(4)->setPen(QPen(Qt::green, 2, Qt::DashLine));
        customPlot->graph(4)->setName("S=1‰");

        for (int i = 0; i < 80; i++)
        {
            customPlot->graph(4)->addData(i, 1);
        }

        customPlot->addGraph();
        customPlot->graph(5)->setPen(QPen(Qt::green, 2, Qt::DashLine));
        customPlot->graph(5)->setName("S=-1‰");

        for (int i = 0; i < 80; i++)
        {
            customPlot->graph(5)->addData(i, -1);
        }

        customPlot->replot();
    }
    else
    {
        PageDataGenerator *model = Singleton<PageDataGenerator>::getInstance(nullptr);
        connect(model, &PageDataGenerator::retrievalCompleted, this, &RetrievalWindow::onRetrievalCompleted);
    }

    // 更新进度条
    ui->progressBar->setValue((m_mesurementError.size() * 100) / Singleton<ConstantStorage>::getInstance(nullptr)->getConstant(constantMap->getConstantName(9, 9)).toInt());
}

QTextEdit *RetrievalWindow::getRetrievalTextEdit()
{
    return this->ui->textEdit3_2;
}

QCustomPlot *RetrievalWindow::getRetrievalCustomPlot()
{
    return this->ui->customPlot3_2;
}

QCustomPlot *RetrievalWindow::getMesurementCustomPlot()
{
    return this->ui->customPlot3_3;
}

// “重置”按钮点击事件
void RetrievalWindow::onClearButtonClicked()
{
    // 清空所有数据
    m_mesurementError.clear();
    m_retrievalError.clear();
    // 清除散点图
    QCustomPlot *customPlot1 = getRetrievalCustomPlot();
    QCustomPlot *customPlot2 = getMesurementCustomPlot();
    customPlot1->clearGraphs();
    customPlot2->clearGraphs();
    customPlot1->replot();
    customPlot2->replot();
}

// 存入lineEdit数据到内存中
void RetrievalWindow::saveLineEditGroupsText()
{
    // 保存所有页面的输入
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);

    // QVector<QString> retrievalConstantVector = QVector<QString>({"initialBrillouinShift ",
    //                                                          "initialBrillouinLine ",
    //                                                          "initialRayleighLine",
    //                                                          "upperBrillouinShift ",
    //                                                          "upperBrillouinLine ",
    //                                                          "upperRayleighLine",
    //                                                          "lowerBrillouinShift ",
    //                                                          "lowerBrillouinLine ",
    //                                                          "lowerRayleighLine"});
    constantStorage->setConstant(constantMap->getConstantName(9, 0), ui->lineEdit_32->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 1), ui->lineEdit_33->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 2), ui->lineEdit_34->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 3), ui->lineEdit_27->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 4), ui->lineEdit_29->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 5), ui->lineEdit_31->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 6), ui->lineEdit_26->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 7), ui->lineEdit_28->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 8), ui->lineEdit_30->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 9), ui->lineEdit_1->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 10), ui->lineEdit_2->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 11), ui->lineEdit_3->text());
    constantStorage->setConstant(constantMap->getConstantName(9, 12), ui->lineEdit->text());
}