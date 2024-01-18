#ifndef CUSTOMPLOTMANAGER_H
#define CUSTOMPLOTMANAGER_H

#include <QObject>
#include "base/singleton.h"
#include "module/qcustomplot.h"

class CustomPlotManager : public QObject
{
    Q_OBJECT
    SINGLETON(CustomPlotManager)
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

    void setCustomPlot(QCustomPlot *newCustomPlot);
    QCustomPlot *getCustomPlot();
    void initCustomPlotStyle();
    void plotGraphToBuffer(const QVector<double> *xData,
                           const QVector<double> *yData,
                           int curve_index);
    void plotGraph(const QVector<double> *xData, const QVector<double> *yData, int curve_index);
    void refreshPlot();
    void clearPlot();
    void hidePlot();
    void showPlot();
    int getCount();
    bool getTracerStatus();
    void setLegendName(const QString &name, int curve_index);
    void createSecondAxis(double lower, double upper, QString label);
    void switchToSecondAxis(int index);
    bool changeTracerStatus();
    void stopGenerateTracerEventSignal();
    void startGenerateTracerEventSignal();

public slots:
    void handleMouseMove(QMouseEvent *event);

public:
signals:
    void mouseMoveSignal(QMouseEvent *event, QVector<QColor> colorVector);

private:
    CustomPlotManager(QCustomPlot *_customPlot, QObject *parent = nullptr);
    QCustomPlot *customPlot;
};

#endif // CUSTOMPLOTMANAGER_H
