#include "mainwindow.h"
#include "QDebug"
#include "base/singleton.h"
#include "controller/plotcontroller.h"
#include "model/pagedatagenerator.h"
#include "ui_mainwindow.h"
#include "view/plotView.h"
#include "model/constantstorage.h"
#include "model/constantmap.h"
#include "singleton.h"

QGroupBox *createParameterGroup(const QString &title, const QMap<QString, QString> &params)
{
    QGroupBox *groupBox = new QGroupBox(title);
    QVBoxLayout *layout = new QVBoxLayout;
    groupBox->setStyleSheet("QGroupBox { font-weight: bold; font-size: 12px; border: 2px solid gray; border-radius: 2px; margin-top: 1ex; }");
    // 设置标题字体大小
    groupBox->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));

    for (auto it = params.constBegin(); it != params.constEnd(); ++it)
    {
        QHBoxLayout *paramLayout = new QHBoxLayout;
        QLabel *label = new QLabel(it.key());
        QLineEdit *lineEdit = new QLineEdit(it.value());
        // 设置lineEdit的样式和宽度
        // lineEdit->setStyleSheet("QLineEdit { border: 1px solid gray; border-radius: 5px; padding: 0 8px; }");
        lineEdit->setReadOnly(true);
        paramLayout->addWidget(label);
        paramLayout->addWidget(lineEdit);
        layout->addLayout(paramLayout);
    }

    groupBox->setLayout(layout);
    return groupBox;
}

void setToolBarContent(QToolBar *toolBar)
{
    // 激光参数组
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();

    QMap<QString, QString> laserParams;
    laserParams[constantMap->getConstantName(0, 0)] = constantStorage->getConstant(constantMap->getConstantName(0, 0)).toString();
    laserParams[constantMap->getConstantName(0, 1)] = constantStorage->getConstant(constantMap->getConstantName(0, 1)).toString();
    laserParams[constantMap->getConstantName(0, 2)] = constantStorage->getConstant(constantMap->getConstantName(0, 2)).toString();
    QGroupBox *laserGroup = createParameterGroup("Laser Parameters", laserParams);

    // 水下环境要素组
    QMap<QString, QString> waterParams;
    waterParams[constantMap->getConstantName(1, 0)] = constantStorage->getConstant(constantMap->getConstantName(1, 0)).toString();
    waterParams[constantMap->getConstantName(1, 1)] = constantStorage->getConstant(constantMap->getConstantName(1, 1)).toString();
    waterParams[constantMap->getConstantName(1, 2)] = constantStorage->getConstant(constantMap->getConstantName(1, 2)).toString();
    waterParams[constantMap->getConstantName(1, 4)] = constantStorage->getConstant(constantMap->getConstantName(1, 4)).toString();
    waterParams[constantMap->getConstantName(7, 1)] = constantStorage->getConstant(constantMap->getConstantName(7, 1)).toString();
    waterParams[constantMap->getConstantName(7, 6)] = constantStorage->getConstant(constantMap->getConstantName(7, 6)).toString();
    QGroupBox *waterGroup = createParameterGroup("Underwater Parameters", waterParams);

    // 探测器参数组
    QMap<QString, QString> detectorParams;
    detectorParams[constantMap->getConstantName(7, 0)] = constantStorage->getConstant(constantMap->getConstantName(7, 0)).toString();
    detectorParams[constantMap->getConstantName(7, 2)] = constantStorage->getConstant(constantMap->getConstantName(7, 2)).toString();
    detectorParams[constantMap->getConstantName(7, 3)] = constantStorage->getConstant(constantMap->getConstantName(7, 3)).toString();
    detectorParams[constantMap->getConstantName(7, 4)] = constantStorage->getConstant(constantMap->getConstantName(7, 4)).toString();
    detectorParams[constantMap->getConstantName(7, 7)] = constantStorage->getConstant(constantMap->getConstantName(7, 7)).toString();
    QGroupBox *detectorGroup = createParameterGroup("Detector Parameters", detectorParams);

    // 菲涅尔参数组
    QMap<QString, QString> fizeauParams;
    fizeauParams[constantMap->getConstantName(2, 0)] = constantStorage->getConstant(constantMap->getConstantName(2, 0)).toString();
    fizeauParams[constantMap->getConstantName(2, 1)] = constantStorage->getConstant(constantMap->getConstantName(2, 1)).toString();
    fizeauParams[constantMap->getConstantName(2, 2)] = constantStorage->getConstant(constantMap->getConstantName(2, 2)).toString();
    fizeauParams[constantMap->getConstantName(2, 3)] = constantStorage->getConstant(constantMap->getConstantName(2, 3)).toString();
    fizeauParams[constantMap->getConstantName(2, 4)] = constantStorage->getConstant(constantMap->getConstantName(2, 4)).toString();
    QGroupBox *fizeauGroup = createParameterGroup("Fizeau Parameters", fizeauParams);

    // PMT参数组
    QMap<QString, QString> pmtParams;
    pmtParams[constantMap->getConstantName(3, 0)] = constantStorage->getConstant(constantMap->getConstantName(3, 0)).toString();
    pmtParams[constantMap->getConstantName(3, 1)] = constantStorage->getConstant(constantMap->getConstantName(3, 1)).toString();
    pmtParams[constantMap->getConstantName(3, 2)] = constantStorage->getConstant(constantMap->getConstantName(3, 2)).toString();
    QGroupBox *pmtGroup = createParameterGroup("PMT Parameters", pmtParams);

    // 添加到工具栏
    QWidget *container = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(laserGroup);
    layout->addWidget(waterGroup);
    layout->addWidget(detectorGroup);
    layout->addWidget(fizeauGroup);
    layout->addWidget(pmtGroup);

    layout->addStretch();
    container->setLayout(layout);

    toolBar->addWidget(container);
    toolBar->setStyleSheet("QToolBar { background-color: #f0f0f0; border: none; }");
}

void setToolBarContent(QWidget *toolBar)
{
    // 激光参数组
    ConstantStorage *constantStorage = Singleton<ConstantStorage>::getInstance(nullptr);
    ConstantMap *constantMap = Singleton<ConstantMap>::getInstance();

    QMap<QString, QString> laserParams;
    laserParams[constantMap->getConstantName(0, 0)] = constantStorage->getConstant(constantMap->getConstantName(0, 0)).toString();
    laserParams[constantMap->getConstantName(0, 1)] = constantStorage->getConstant(constantMap->getConstantName(0, 1)).toString();
    laserParams[constantMap->getConstantName(0, 2)] = constantStorage->getConstant(constantMap->getConstantName(0, 2)).toString();
    QGroupBox *laserGroup = createParameterGroup("Laser Parameters", laserParams);

    // 水下环境要素组
    QMap<QString, QString> waterParams;
    waterParams[constantMap->getConstantName(1, 0)] = constantStorage->getConstant(constantMap->getConstantName(1, 0)).toString();
    waterParams[constantMap->getConstantName(1, 1)] = constantStorage->getConstant(constantMap->getConstantName(1, 1)).toString();
    // waterParams[constantMap->getConstantName(1, 2)] = constantStorage->getConstant(constantMap->getConstantName(1, 2)).toString();
    waterParams[constantMap->getConstantName(1, 4)] = constantStorage->getConstant(constantMap->getConstantName(1, 4)).toString();
    waterParams[constantMap->getConstantName(7, 1)] = constantStorage->getConstant(constantMap->getConstantName(7, 1)).toString();
    waterParams[constantMap->getConstantName(7, 6)] = constantStorage->getConstant(constantMap->getConstantName(7, 6)).toString();
    QGroupBox *waterGroup = createParameterGroup("Underwater Parameters", waterParams);

    // 探测器参数组
    QMap<QString, QString> detectorParams;
    detectorParams[constantMap->getConstantName(7, 0)] = constantStorage->getConstant(constantMap->getConstantName(7, 0)).toString();
    detectorParams[constantMap->getConstantName(7, 2)] = constantStorage->getConstant(constantMap->getConstantName(7, 2)).toString();
    detectorParams[constantMap->getConstantName(7, 3)] = constantStorage->getConstant(constantMap->getConstantName(7, 3)).toString();
    detectorParams[constantMap->getConstantName(7, 4)] = constantStorage->getConstant(constantMap->getConstantName(7, 4)).toString();
    detectorParams[constantMap->getConstantName(7, 7)] = constantStorage->getConstant(constantMap->getConstantName(7, 7)).toString();
    QGroupBox *detectorGroup = createParameterGroup("Detector Parameters", detectorParams);

    // 菲涅尔参数组
    QMap<QString, QString> fizeauParams;
    fizeauParams[constantMap->getConstantName(2, 0)] = constantStorage->getConstant(constantMap->getConstantName(2, 0)).toString();
    fizeauParams[constantMap->getConstantName(2, 1)] = constantStorage->getConstant(constantMap->getConstantName(2, 1)).toString();
    fizeauParams[constantMap->getConstantName(2, 2)] = constantStorage->getConstant(constantMap->getConstantName(2, 2)).toString();
    fizeauParams[constantMap->getConstantName(2, 3)] = constantStorage->getConstant(constantMap->getConstantName(2, 3)).toString();
    fizeauParams[constantMap->getConstantName(2, 4)] = constantStorage->getConstant(constantMap->getConstantName(2, 4)).toString();
    QGroupBox *fizeauGroup = createParameterGroup("Fizeau Parameters", fizeauParams);

    // PMT参数组
    QMap<QString, QString> pmtParams;
    pmtParams[constantMap->getConstantName(3, 0)] = constantStorage->getConstant(constantMap->getConstantName(3, 0)).toString();
    pmtParams[constantMap->getConstantName(3, 1)] = constantStorage->getConstant(constantMap->getConstantName(3, 1)).toString();
    pmtParams[constantMap->getConstantName(3, 2)] = constantStorage->getConstant(constantMap->getConstantName(3, 2)).toString();
    QGroupBox *pmtGroup = createParameterGroup("PMT Parameters", pmtParams);

    // 添加到工具栏
    QWidget *container = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(laserGroup);
    layout->addWidget(waterGroup);
    layout->addWidget(detectorGroup);
    layout->addWidget(fizeauGroup);
    layout->addWidget(pmtGroup);

    layout->addStretch();
    container->setLayout(layout);

    // toolBar->addWidget(container);
    toolBar->layout()->addWidget(container);
    toolBar->setStyleSheet("QToolBar { background-color: #f0f0f0; border: none; }");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 获取屏幕大小
    QDesktopWidget *desktop = QApplication::desktop();
    int screenCount = desktop->screenCount();
    // 获取屏幕大小
    QRect rect = desktop->screenGeometry();
    // 获取屏幕大小
    int width = rect.width();
    int height = rect.height();
    // 设置窗口大小
    // this->resize(0.8 * width, 0.8 * height);
    this->resize(1600, 1200);
    // 挂载视图层
    // 挂载视图层
    Singleton<PlotView>::getInstance(ui);
    // 挂载模型层
    Singleton<PageDataGenerator>::getInstance(nullptr);
    // 挂载控制层
    Singleton<PlotController>::getInstance(Singleton<PlotView>::getInstance(),
                                           Singleton<PageDataGenerator>::getInstance(nullptr));

    // 在CentralWidget中添加一个垂直布局的Widget，用来放ParameterGroup
    QWidget *leftWidget = new QWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftWidget->setLayout(leftLayout);

    // 检查 ui->centralWidget 的布局是否为 QBoxLayout 类型
    QBoxLayout *layout = qobject_cast<QBoxLayout *>(ui->widget_2->layout());
    if (layout)
    {
        // 将 leftWidget 插入到第一个位置，即左边
        layout->insertWidget(0, leftWidget);
        layout->setStretch(0, 1); // 设置 leftWidget 的 stretch
        layout->setStretch(1, 6); // 设置 ui->centralWidget 的 stretch
    }
    else
    {
        // 如果布局不是 QBoxLayout 或不存在，创建一个新的水平布局
        QHBoxLayout *mainLayout = new QHBoxLayout;
        mainLayout->addWidget(leftWidget);
        mainLayout->addWidget(ui->widget_2);

        // 设置 leftWidget 的 stretch 比例，例如 1：3 的比例
        mainLayout->setStretch(0, 1); // 设置 leftWidget 的 stretch
        mainLayout->setStretch(1, 5); // 设置 ui->centralWidget 的 stretch

        // 设置新布局给 stackedWidget的第0页
        // ui->centralWidget->setLayout(mainLayout);
        ui->widget_2->setLayout(mainLayout);
    }

    // 设置工具栏内容
    setToolBarContent(leftWidget);

    // QMainWindow &mainWindow = *this;
    // QToolBar *toolBar = nullptr; // 初始化为空指针

    // // 初始创建工具栏
    // toolBar = new QToolBar("Parameters", &mainWindow);
    // mainWindow.addToolBar(Qt::LeftToolBarArea, toolBar);

    // // 设置工具栏内容
    // setToolBarContent(toolBar);

    // // 设置工具栏的高度
    // toolBar->setMaximumHeight(0.9 * height);
    // toolBar->setFixedWidth(484);

    // connect(Singleton<PlotView>::getInstance(), &PlotView::switchPageButtonClicked, this, [=](int page_index, QRect area, int from_pagex) mutable
    //         {
    // if (page_index != 0) {
    //     // 删除旧的工具栏
    //     if (toolBar != nullptr) {
    //         delete toolBar;
    //         toolBar = nullptr;
    //     }
    // } else {
    //     QMainWindow &mainWindow = *this;
    //     // 再次创建工具栏
    //     toolBar = new QToolBar("Parameters", &mainWindow);
    //     mainWindow.addToolBar(Qt::LeftToolBarArea, toolBar);

    //     // 设置工具栏内容
    //     setToolBarContent(toolBar);

    //     // 设置工具栏的高度
    //     toolBar->setMaximumHeight(1620);
    //     toolBar->setFixedWidth(484);
    // } }, Qt::DirectConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//调用installTranslator后，系统会给窗体发送信号将产生了changeEvent槽产生event
void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
