#ifndef CUSTOMPLOTMANAGER_H
#define CUSTOMPLOTMANAGER_H

#include "qcustomplot.h"

class CustomPlotManager
{
public:
    QVector<QColor> colorContainer = {QColor(226, 60, 255),
                                      QColor(64, 224, 208),
                                      QColor(255, 99, 71),
                                      QColor(255, 255, 0),
                                      QColor(0, 255, 255),
                                      QColor(255, 0, 0),
                                      QColor(0, 255, 0),
                                      QColor(0, 0, 255),
                                      QColor(255, 255, 255),
                                      QColor(0, 0, 0)};

    static CustomPlotManager *getCustomPlotManagerInstance(QCustomPlot *customPlot);
    static CustomPlotManager *getCustomPlotManagerInstance();

    void setCustomPlot(QCustomPlot *newCustomPlot);
    QCustomPlot *getCustomPlot();
    void initCustomPlotStyle();
    void plotGraph(const QVector<double> *xData, const QVector<double> *yData, int curve_num);
    void createSecondAxis(double lower, double upper, QString label);
    void switchToSecondAxis(int index);

private:
    CustomPlotManager(QCustomPlot *_customPlot);

    static CustomPlotManager *customPlotManagerInstance;
    QCustomPlot *customPlot;
};

#endif // CUSTOMPLOTMANAGER_H
