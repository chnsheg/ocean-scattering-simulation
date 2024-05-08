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

    for (auto &&customPlot : ui->centralwidget->findChildren<QCustomPlot *>())
    {
        Singleton<CustomPlotManager>::getInstance()->setCustomPlot(customPlot); // Tracer可能会出问题，只会锚定到最后一个customPlot上
    }
    Singleton<TextEditManager>::getInstance()->setTextEdit(getRetrievalTextEdit());
    Singleton<TextEditManager>::getInstance()->initTextEditStyle();
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
}

// “显示”按钮点击事件
void RetrievalWindow::onShowButtonClicked()
{
    PageDataGenerator *model = Singleton<PageDataGenerator>::getInstance(nullptr);
    // 绑定反演结束信号
    connect(model, &PageDataGenerator::retrievalCompleted, this, &RetrievalWindow::onRetrievalCompleted);
    model->generateDynamicAction(0);
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
        double max_y = std::abs(m_retrievalError.at(0).temperatureError);

        for (auto &&retrievalError : m_retrievalError)
        {
            temperatureError.push_back(retrievalError.temperatureError);
            salinityError.push_back(retrievalError.salinityError);
            retrievalTimes.push_back(retrievalTimes.size() + 1);
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
        customPlot->xAxis->setRange(0, retrievalTimes.size() + 1);
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
        double max_y = std::abs(m_mesurementError.at(0).BrillouinShiftError);

        for (auto &&mesurementError : m_mesurementError)
        {
            BrillouinShiftError.push_back(mesurementError.BrillouinShiftError);
            BrillouinWidthError.push_back(mesurementError.BrillouinWidthError);
            rayleighWidthError.push_back(mesurementError.rayleighWidthError);
            retrievalTimes.push_back(retrievalTimes.size() + 1);
            qDebug() << "MesurementError: " << mesurementError.BrillouinShiftError << " " << mesurementError.BrillouinWidthError << " " << mesurementError.rayleighWidthError;

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
        customPlot->xAxis->setRange(0, retrievalTimes.size() + 1);
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

    m_mesurementError.push_back(mesurementError);
    m_retrievalError.push_back(retrievalError);

    // 绘制散点图
    drawMesurementErrorScatterPlot();
    drawRetrievalErrorScatterPlot();
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
    // TODO
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
}