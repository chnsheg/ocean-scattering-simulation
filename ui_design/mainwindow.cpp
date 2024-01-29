#include "mainwindow.h"
#include "QDebug"
#include "base/singleton.h"
#include "controller/plotcontroller.h"
#include "model/pagedatagenerator.h"
#include "ui_mainwindow.h"
#include "view/plotView.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 挂载视图层
    Singleton<PlotView>::getInstance(ui);
    // 挂载模型层
    Singleton<PageDataGenerator>::getInstance(nullptr);
    // 挂载控制层
    Singleton<PlotController>::getInstance(Singleton<PlotView>::getInstance(),
                                           Singleton<PageDataGenerator>::getInstance(nullptr));
}

MainWindow::~MainWindow()
{
    delete ui;
}
