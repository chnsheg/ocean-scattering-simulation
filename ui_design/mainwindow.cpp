#include "mainwindow.h"
#include "QDebug"
#include "ui_mainwindow.h"

void qt_read_csv_test(QVector<double> *data, QString filename)
{
    QFile inFile(":/" + filename);
    QStringList lines; /*行数据*/
    if (inFile.open(QIODevice::ReadOnly)) {
        QTextStream stream_text(&inFile);
        while (!stream_text.atEnd()) {
            lines.push_back(stream_text.readLine());
        }
        for (int j = 0; j < lines.size(); j++) {
            QString line = lines.at(j);
            QStringList split = line.split(","); /*列数据*/
            for (int col = 0; col < split.size(); col++) {
                data->push_back(split.at(col).toDouble());
                // qDebug() << split.at(col).toDouble() << "\t";
            }
        }
        inFile.close();
    } else {
        qDebug() << "open file failed";
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadHomeScreen();
    // int index = ui->stackedWidget->currentIndex();
    // customPlot = ui->stackedWidget->findChild<QCustomPlot *>(QString("customPlot%1").arg(index));
    // if (customPlot != nullptr) {
    //     InitializePlot(customPlot);
    //     loadTracer(customPlot);
    //     loadFunctionButton();
    // }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*********************曲线配置************************/
void MainWindow::InitializePlot(QCustomPlot *CustomPlot)
{
    CustomPlot->xAxis->setLabel("x");                         //设置x轴名称
    CustomPlot->yAxis->setLabel("y");                         //设置y轴名称
    CustomPlot->xAxis->setLabelColor(QColor(226, 60, 255));   //设置x轴名称颜色
    CustomPlot->yAxis->setLabelColor(QColor(226, 60, 255));   //设置y轴名称颜色
    CustomPlot->xAxis->setTickLabelColor(Qt::yellow);         //设置x轴坐标颜色
    CustomPlot->yAxis->setTickLabelColor(Qt::yellow);         //设置y轴坐标颜色
    CustomPlot->xAxis->setBasePen(QPen(QColor(0, 0, 0)));     //设置x轴坐标轴颜色
    CustomPlot->yAxis->setBasePen(QPen(QColor(25, 150, 92))); //设置y轴坐标轴颜色
    //设置画布背景色
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    CustomPlot->setBackground(plotGradient);

    // 显示图例
    CustomPlot->legend->setVisible(true);
    // 设置图例的位置
    CustomPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    //设置legend的字体大小
    CustomPlot->legend->setFont(QFont("Consolas", 12));

    //CustomPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes|QCP::iSelectLegend | QCP::iSelectPlottables);//如有多条曲线可选中，暂时只有一条曲线，先不用这个功能，因为选中会卡一些
    // CustomPlot->setInteractions(QCP::iRangeDrag
    //                             | QCP::iRangeZoom); //可以进行鼠标位置 放大缩小 拖拽  放大缩小坐标系
    CustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes
                                | QCP::iSelectLegend | QCP::iSelectPlottables);
    //blog.csdn.net/qq_40194498/article/details/79741197
    // CustomPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    CustomPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    CustomPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    CustomPlot->axisRect()->setRangeZoomAxes(CustomPlot->xAxis, nullptr);
    CustomPlot->axisRect()->setRangeZoomFactor(0.8);
    CustomPlot->replot(); //重绘 每次改变完以后都要调用这个进行重新绘制
}

void MainWindow::plotGraph(QCustomPlot *CustomPlot,
                           QVector<double> *xtemp,
                           QVector<double> *ytemp,
                           int count)
{
    CustomPlot->addGraph();
    auto maxElement = std::min_element(ytemp->begin(), ytemp->end(), [](double a, double b) {
        return (a >= b);
    });
    CustomPlot->xAxis->setRange(-12, 12);
    CustomPlot->yAxis->setRange(0, *maxElement);
    CustomPlot->graph(count)->setPen(QPen(colorContainer.at(count), 3)); //设置曲线颜色
    CustomPlot->graph(count)->setData(*xtemp, *ytemp);
    CustomPlot->replot(); //重绘 每次改变完以后都要调用这个进行重新绘制
}

void MainWindow::loadTracer(QCustomPlot *CustomPlot)
{
    //在构造的过程中虽然有new，但是最后释放了，所以不需要手动释放
    m_TracerY = QSharedPointer<myTracer>(new myTracer(
        CustomPlot,
        CustomPlot->graph(0),
        DataTracer)); //这行代码是关键，设置鼠标跟踪器，DataTracer是枚举类型，可以设置为XAxisTracer，YAxisTracer，DataTracer
    //m_TraserX = QSharedPointer<myTracer> (new myTracer(CustomPlot, CustomPlot->graph(0), XAxisTracer));
    // connect(CustomPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(showTracer(QMouseEvent *)));
}

void MainWindow::loadHomeScreen()
{
    connect(ui->homeButton_1, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(1);
        customPlot = ui->stackedWidget->findChild<QCustomPlot *>(QString("customPlot1"));
        if (customPlot != nullptr) {
            InitializePlot(customPlot);
            loadFunctionButton();
        }
    });
    connect(ui->homeButton_2, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(2);
        customPlot = ui->stackedWidget->findChild<QCustomPlot *>(QString("customPlot2"));
        if (customPlot != nullptr) {
            InitializePlot(customPlot);
            loadFunctionButton();
        }
    });
    connect(ui->homeButton_3, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(3);
        customPlot = ui->stackedWidget->findChild<QCustomPlot *>(QString("customPlot3"));
        if (customPlot != nullptr) {
            InitializePlot(customPlot);
            loadFunctionButton();
        }
    });
    connect(ui->homeButton_4, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(4);
        customPlot = ui->stackedWidget->findChild<QCustomPlot *>(QString("customPlot4"));
        if (customPlot != nullptr) {
            InitializePlot(customPlot);
            loadFunctionButton();
        }
    });
    connect(ui->homeButton_5, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(5);
        customPlot = ui->stackedWidget->findChild<QCustomPlot *>(QString("customPlot5"));
        if (customPlot != nullptr) {
            InitializePlot(customPlot);
            loadFunctionButton();
        }
    });
}

static int times = 0;

void MainWindow::loadFunctionButton()
{
    ui->show1->setEnabled(true);
    ui->show2->setEnabled(true);
    ui->show3->setEnabled(true);
    ui->show4->setEnabled(true);
    ui->show5->setEnabled(true);
    connect(ui->show1, &QPushButton::clicked, this, &MainWindow::start_plot, Qt::UniqueConnection);
    connect(ui->show2, &QPushButton::clicked, this, &MainWindow::start_plot, Qt::UniqueConnection);
    connect(ui->show3, &QPushButton::clicked, this, &MainWindow::start_plot, Qt::UniqueConnection);
    connect(ui->show4, &QPushButton::clicked, this, &MainWindow::start_plot, Qt::UniqueConnection);
    connect(ui->show5, &QPushButton::clicked, this, &MainWindow::start_plot, Qt::UniqueConnection);

    ui->clear1->setEnabled(false);
    ui->clear2->setEnabled(false);
    ui->clear3->setEnabled(false);
    ui->clear4->setEnabled(false);
    ui->clear5->setEnabled(false);
    connect(ui->clear1, &QPushButton::clicked, this, &MainWindow::clear_plot, Qt::UniqueConnection);
    connect(ui->clear2, &QPushButton::clicked, this, &MainWindow::clear_plot, Qt::UniqueConnection);
    connect(ui->clear3, &QPushButton::clicked, this, &MainWindow::clear_plot, Qt::UniqueConnection);
    connect(ui->clear4, &QPushButton::clicked, this, &MainWindow::clear_plot, Qt::UniqueConnection);
    connect(ui->clear5, &QPushButton::clicked, this, &MainWindow::clear_plot, Qt::UniqueConnection);

    //切换光标是否显示，默认为显示
    ui->tracer1->setEnabled(false);
    ui->tracer2->setEnabled(false);
    ui->tracer3->setEnabled(false);
    ui->tracer4->setEnabled(false);
    ui->tracer5->setEnabled(false);
    if (m_TracerY != nullptr) {
        m_TracerY->setVisible(false);
    }
    connect(ui->tracer1, &QPushButton::clicked, this, &MainWindow::hide_trace, Qt::UniqueConnection);
    connect(ui->tracer2, &QPushButton::clicked, this, &MainWindow::hide_trace, Qt::UniqueConnection);
    connect(ui->tracer3, &QPushButton::clicked, this, &MainWindow::hide_trace, Qt::UniqueConnection);
    connect(ui->tracer4, &QPushButton::clicked, this, &MainWindow::hide_trace, Qt::UniqueConnection);
    connect(ui->tracer5, &QPushButton::clicked, this, &MainWindow::hide_trace, Qt::UniqueConnection);
    connect(ui->back1, &QPushButton::clicked, this, [=]() {
        customPlot->clearGraphs();
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->back2, &QPushButton::clicked, this, [=]() {
        customPlot->clearGraphs();
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->back3, &QPushButton::clicked, this, [=]() {
        customPlot->clearGraphs();
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->back4, &QPushButton::clicked, this, [=]() {
        customPlot->clearGraphs();
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->back5, &QPushButton::clicked, this, [=]() {
        customPlot->clearGraphs();
        ui->stackedWidget->setCurrentIndex(0);
    });
}

void MainWindow::start_plot()
{
    int index = ui->stackedWidget->currentIndex();
    if (customPlot != nullptr) {
        customPlot->clearGraphs();
        //现在我在写一个按键的槽函数void MainWindow::start_plot()，他的功能是根据当前ui->stackedWidget->currentIndex()的值确定当前页面，然后对应显示各个页面的曲线，已知第一个页面只有一条曲线，第二个页面有3条，第三个页面有一条，曲线的x、y坐标都是存储在堆区的，请问这个代码 逻辑怎么写
        switch (index) {
        case 1:
            //设置当前界面功能按键
            ui->show1->setEnabled(false);
            ui->clear1->setEnabled(true);
            ui->tracer1->setEnabled(true);
            //读取数据
            RF = new QVector<double>;
            laser_y = new QVector<double>;
            qt_read_csv_test(RF, "RF.csv");
            qt_read_csv_test(laser_y, "laser_data.csv");
            //绘制曲线
            plotGraph(customPlot, RF, laser_y);
            customPlot->graph(0)->setName("激光光谱");
            break;
        case 2:
            ui->show2->setEnabled(false);
            ui->clear2->setEnabled(true);
            ui->tracer2->setEnabled(true);
            RF = new QVector<double>;
            L_mc = new QVector<double>;
            L_rc = new QVector<double>;
            L_bc = new QVector<double>;
            qt_read_csv_test(RF, "RF.csv");
            qt_read_csv_test(L_mc, "L_mc.csv");
            qt_read_csv_test(L_rc, "L_rc.csv");
            qt_read_csv_test(L_bc, "L_bc.csv");
            plotGraph(customPlot, RF, L_bc);
            customPlot->graph(0)->setName("布里渊散射曲线");
            plotGraph(customPlot, RF, L_rc, 1);
            customPlot->graph(1)->setName("瑞利散射曲线");
            plotGraph(customPlot, RF, L_mc, 2);
            customPlot->graph(2)->setName("米散射曲线");
            break;
        case 3:
            ui->show3->setEnabled(false);
            ui->clear3->setEnabled(true);
            ui->tracer3->setEnabled(true);
            RF = new QVector<double>;
            Spectrum1 = new QVector<double>;
            qt_read_csv_test(RF, "RF.csv");
            qt_read_csv_test(Spectrum1, "Spectrum1.csv");
            plotGraph(customPlot, RF, Spectrum1);
            customPlot->graph(0)->setName("水下散射光谱");
            break;
        case 4:
            ui->show4->setEnabled(false);
            ui->clear4->setEnabled(true);
            ui->tracer4->setEnabled(true);
            RF = new QVector<double>;
            Fizeau_spectrum = new QVector<double>;
            SpectrumAfterFizeau = new QVector<double>;
            qt_read_csv_test(RF, "RF.csv");
            qt_read_csv_test(Fizeau_spectrum, "Fizeau_spectrum.csv");
            qt_read_csv_test(SpectrumAfterFizeau, "SpectrumAfterFizeau.csv");
            // QCPAxis *axis2 = new QCPAxis(customPlot, QCPAxis::atRight);
            // customPlot->axisRect()->addAxis(axis2);
            //创建第二条坐标轴
            customPlot->yAxis2->setVisible(true);
            customPlot->yAxis2->setTickLabels(true);
            customPlot->yAxis2->setRange(0, 1);
            customPlot->yAxis2->setTickLabelColor(Qt::yellow);
            customPlot->yAxis2->setLabelColor(QColor(226, 60, 255));
            //设置第二条坐标轴可以拖动
            customPlot->yAxis2->setLabel("y");
            plotGraph(customPlot, RF, Fizeau_spectrum);
            customPlot->graph(0)->setName("Fizeau仪器函数");
            customPlot->graph(0)->setValueAxis(customPlot->yAxis2);
            plotGraph(customPlot, RF, SpectrumAfterFizeau, 1);
            customPlot->graph(1)->setName("通过Fizeau后的光谱");
            break;
        case 5:
            ui->show5->setEnabled(false);
            ui->clear5->setEnabled(true);
            ui->tracer5->setEnabled(true);
            PMT_Sign = new QVector<double>;
            PMT_Energy = new QVector<double>;
            qt_read_csv_test(PMT_Sign, "PMT_Sign.csv");
            qt_read_csv_test(PMT_Energy, "PMT_Energy.csv");
            plotGraph(customPlot, PMT_Sign, PMT_Energy);
            customPlot->graph(0)->setName("PMT能谱");
            break;
        default:
            break;
        }
        //加载光标
        loadTracer(customPlot);
        m_TracerY->setVisible(true);
        connect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(showTracer(QMouseEvent *)));
    }
}

void MainWindow::clear_plot()
{
    int index = ui->stackedWidget->currentIndex();
    //取消Tracer，并取消原来的显示
    disconnect(customPlot, SIGNAL(mouseMove(QMouseEvent *)), this, SLOT(showTracer(QMouseEvent *)));
    m_TracerY->setVisible(false);

    switch (index) {
    case 1:
        //使当前界面的show按键不可用
        ui->clear1->setEnabled(false);
        ui->show1->setEnabled(true);
        ui->tracer1->setText("隐藏光标");
        ui->tracer1->setEnabled(false);
        delete RF;
        delete laser_y;
        break;
    case 2:
        ui->clear2->setEnabled(false);
        ui->show2->setEnabled(true);
        ui->tracer2->setText("隐藏光标");
        ui->tracer2->setEnabled(false);
        delete RF;
        delete L_mc;
        delete L_rc;
        delete L_bc;
        break;
    case 3:
        ui->clear3->setEnabled(false);
        ui->show3->setEnabled(true);
        ui->tracer3->setText("隐藏光标");
        ui->tracer3->setEnabled(false);
        delete RF;
        delete Spectrum1;
        break;
    case 4:
        ui->clear4->setEnabled(false);
        ui->show4->setEnabled(true);
        ui->tracer4->setText("隐藏光标");
        ui->tracer4->setEnabled(false);
        delete RF;
        delete Fizeau_spectrum;
        delete SpectrumAfterFizeau;
        break;
    case 5:
        ui->clear5->setEnabled(false);
        ui->show5->setEnabled(true);
        ui->tracer5->setText("隐藏光标");
        ui->tracer5->setEnabled(false);
        delete PMT_Energy;
        delete PMT_Sign;
        break;
    default:
        break;
    }
    customPlot->clearGraphs();
    customPlot->replot();
}

void MainWindow::hide_trace()
{
    int index = ui->stackedWidget->currentIndex();

    switch (index) {
    case 1:
        if (ui->tracer1->text() == "隐藏光标") {
            ui->tracer1->setText("显示光标");
            m_TracerY->setVisible(false);
            disconnect(customPlot,
                       SIGNAL(mouseMove(QMouseEvent *)),
                       this,
                       SLOT(showTracer(QMouseEvent *)));
            customPlot->replot();
        } else {
            ui->tracer1->setText("隐藏光标");
            m_TracerY->setVisible(true);
            connect(customPlot,
                    SIGNAL(mouseMove(QMouseEvent *)),
                    this,
                    SLOT(showTracer(QMouseEvent *)));
        }
        break;
    case 2:
        if (ui->tracer2->text() == "隐藏光标") {
            ui->tracer2->setText("显示光标");
            m_TracerY->setVisible(false);
            disconnect(customPlot,
                       SIGNAL(mouseMove(QMouseEvent *)),
                       this,
                       SLOT(showTracer(QMouseEvent *)));
            customPlot->replot();
            customPlot->replot();
        } else {
            ui->tracer2->setText("隐藏光标");
            m_TracerY->setVisible(true);
            connect(customPlot,
                    SIGNAL(mouseMove(QMouseEvent *)),
                    this,
                    SLOT(showTracer(QMouseEvent *)));
        }
        break;
    case 3:
        if (ui->tracer3->text() == "隐藏光标") {
            ui->tracer3->setText("显示光标");
            m_TracerY->setVisible(false);
            disconnect(customPlot,
                       SIGNAL(mouseMove(QMouseEvent *)),
                       this,
                       SLOT(showTracer(QMouseEvent *)));
            customPlot->replot();
        } else {
            ui->tracer3->setText("隐藏光标");
            m_TracerY->setVisible(true);
            connect(customPlot,
                    SIGNAL(mouseMove(QMouseEvent *)),
                    this,
                    SLOT(showTracer(QMouseEvent *)));
        }
        break;
    case 4:
        if (ui->tracer4->text() == "隐藏光标") {
            ui->tracer4->setText("显示光标");
            m_TracerY->setVisible(false);
            disconnect(customPlot,
                       SIGNAL(mouseMove(QMouseEvent *)),
                       this,
                       SLOT(showTracer(QMouseEvent *)));
            customPlot->replot();
        } else {
            ui->tracer4->setText("隐藏光标");
            m_TracerY->setVisible(true);
            connect(customPlot,
                    SIGNAL(mouseMove(QMouseEvent *)),
                    this,
                    SLOT(showTracer(QMouseEvent *)));
        }
        break;
    case 5:
        if (ui->tracer5->text() == "隐藏光标") {
            ui->tracer5->setText("显示光标");
            m_TracerY->setVisible(false);
            disconnect(customPlot,
                       SIGNAL(mouseMove(QMouseEvent *)),
                       this,
                       SLOT(showTracer(QMouseEvent *)));
            customPlot->replot();
        } else {
            ui->tracer5->setText("隐藏光标");
            m_TracerY->setVisible(true);
            connect(customPlot,
                    SIGNAL(mouseMove(QMouseEvent *)),
                    this,
                    SLOT(showTracer(QMouseEvent *)));
        }
        break;
    default:
        break;
    }
    customPlot->replot();
}

void MainWindow::showTracer(QMouseEvent *event)
{
    // QElapsedTimer timer;
    // qint64 elapsedTime;
    double x = customPlot->xAxis->pixelToCoord(event->pos().x());
    double mouse_y = customPlot->yAxis->pixelToCoord(event->pos().y());
    //for(int i=0;i<1;i++)//ui->widget9->graph(0)->dataCount()
    //{
    // double y = 0;

    QSharedPointer<QCPGraphDataContainer> tmpContainer;
    //获取customPlot的graph数量
    int graphCount = customPlot->graphCount();
    QVector<QSharedPointer<QCPGraphDataContainer>> tmpContainerVector;
    QVector<double> y;
    for (int i = 0; i < graphCount; ++i) {
        // tmpContainerVector.push_back(customPlot->graph(i)->data());
        tmpContainerVector.push_back(customPlot->graph(i)->data());
    }
    tmpContainer = customPlot->graph(0)->data();
    //使用二分法快速查找所在点数据！！！实测耗时与索引查找相当，二选一即可
    int low = 0, high = tmpContainer->size();
    // timer.start();
    while (high > low) {
        int middle = (low + high) / 2;
        if (x < tmpContainer->constBegin()->mainKey()
            || x > (tmpContainer->constEnd() - 1)->mainKey())
            break;

        if (x == (tmpContainer->constBegin() + middle)->mainKey()) {
            for (int j = 0; j < graphCount; ++j) {
                y.push_back(tmpContainerVector.at(j)->constBegin()->mainValue() + middle);
            }
            break;
        }
        if (x > (tmpContainer->constBegin() + middle)->mainKey()) {
            low = middle;
        } else if (x < (tmpContainer->constBegin() + middle)->mainKey()) {
            high = middle;
        }
        if (high - low <= 1) { //差值计算所在位置数据
            for (int j = 0; j < graphCount; ++j) {
                y.push_back((tmpContainerVector.at(j)->constBegin() + low)->mainValue()
                            + ((x - (tmpContainerVector.at(j)->constBegin() + low)->mainKey())
                               * ((tmpContainerVector.at(j)->constBegin() + high)->mainValue()
                                  - (tmpContainerVector.at(j)->constBegin() + low)->mainValue()))
                                  / ((tmpContainerVector.at(j)->constBegin() + high)->mainKey()
                                     - (tmpContainerVector.at(j)->constBegin() + low)->mainKey()));
            }
            break;
        }
    }
    // int x_index = 0;
    // if (x >= -12 && x <= 12) {
    //     x_index = static_cast<int>((x - (-12)) * 1e3);

    //     for (int j = 0; j < graphCount; ++j) {
    //         y.push_back((tmpContainerVector.at(j)->constBegin() + x_index)->mainValue());
    //     }
    // }
    //从y中获取与mouse_y最接近的数据
    double y_min = 0.0;
    int y_index = 0;
    if (y.size() != 0) {
        y_min = y.at(0);
        for (int i = 1; i < y.size(); ++i) {
            if (fabs(y.at(i) - mouse_y) < fabs(y_min - mouse_y)) {
                y_min = y.at(i);
                y_index = i;
            }
        }
    }
    //判断第y_index个曲线的y轴是否是第二个坐标轴
    // if (customPlot->graph(y_index)->valueAxis() == customPlot->yAxis2) {
    //     //根据第一个坐标轴和第二个坐标轴的比例，将y_min映射到第二个坐标轴上
    //     y_min = y_min * customPlot->yAxis->range().size() / customPlot->yAxis2->range().size();
    // }
    // 将上述if判断转换为三元表达式
    customPlot->graph(y_index)->valueAxis() == customPlot->yAxis2
        ? y_min = y_min
                  * static_cast<double>(customPlot->yAxis->range().size()
                                        / customPlot->yAxis2->range().size())
        : y_min = y_min;
    m_TracerY->updatePosition(x, y_min);
    // m_TracerY->setText(QString::number(x, 'f', 13),
    //                    QString::number(y, 'f', 13)); //x轴取整数，y轴保留两位小数

    m_TracerY->setPen(QPen(colorContainer.at(y_index), 3));
    m_TracerY->setBrush(QBrush(colorContainer.at(y_index)));
    m_TracerY->setLabelPen(QPen(colorContainer.at(y_index), 3));
    // m_TracerY->setText(QString::number(x), QString::number(y_min));
    //使用三元表达式替换m_TracerY->setText(QString::number(x), QString::number(y_min));
    customPlot->graph(y_index)->valueAxis() == customPlot->yAxis2
        ? m_TracerY->setText(QString::number(x),
                             QString::number(
                                 y_min
                                 * static_cast<double>(customPlot->yAxis2->range().size()
                                                       / customPlot->yAxis->range().size())))
        : m_TracerY->setText(QString::number(x), QString::number(y_min));

    //}
    // timer.start();
    customPlot->replot();
    // elapsedTime = timer.elapsed();
    // qDebug() << "showTracer execution time: " << elapsedTime << " milliseconds";
}
