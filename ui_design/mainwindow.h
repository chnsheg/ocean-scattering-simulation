#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mytracer.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //曲线部分
    // void InitializePlot(QCustomPlot *CustomPlot); //初始化绘图区
    // void loadTracer(QCustomPlot *CustomPlot);
    // void loadFunctionButton();
    // void loadHomeScreen();

    // private slots:
    //     void plotGraph(QCustomPlot *CustomPlot,
    //                    QVector<double> *xtemp,
    //                    QVector<double> *ytemp,
    //                    int count = 0); //绘制曲线
    //     void showTracer(QMouseEvent *event);
    //     void start_plot();
    //     void clear_plot();
    //     void hide_trace();

private:
    Ui::MainWindow *ui;
    // QCustomPlot *customPlot; //指向当前页面的QCustomPlot
    // QSharedPointer<myTracer> m_TraserX;
    // QSharedPointer<myTracer> m_TracerY;
    // QVector<double> *RF, *laser_y, *L_mc, *L_rc, *L_bc, *Spectrum1, *PMT_Energy, *PMT_Sign,
    //     *Fizeau_spectrum, *SpectrumAfterFizeau;

    // QVector<QColor> colorContainer = {QColor(226, 60, 255),
    //                                   QColor(64, 224, 208),
    //                                   QColor(255, 99, 71),
    //                                   QColor(255, 255, 0),
    //                                   QColor(0, 255, 255),
    //                                   QColor(255, 0, 0),
    //                                   QColor(0, 255, 0),
    //                                   QColor(0, 0, 255),
    //                                   QColor(255, 255, 255),
    //                                   QColor(0, 0, 0)};
};
#endif // MAINWINDOW_H
