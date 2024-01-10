#ifndef CUSTOMPLOTMANAGER_H
#define CUSTOMPLOTMANAGER_H

#include "qcustomplot.h"

class CustomPlotManager
{
public:
    static CustomPlotManager *getCustomPlotManagerInstance(QCustomPlot *customPlot);
    static CustomPlotManager *getCustomPlotManagerInstance();

    void setCustomPlot(QCustomPlot *newCustomPlot);
    QCustomPlot *getCustomPlot();
    void initCustomPlotStyle();
    void createSecondAxis(double lower, double upper, QString label);
    void switchToSecondAxis(int index);

private:
    CustomPlotManager(QCustomPlot *_customPlot);

    static CustomPlotManager *customPlotManagerInstance;
    QCustomPlot *customPlot;
};

#endif // CUSTOMPLOTMANAGER_H
