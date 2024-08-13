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