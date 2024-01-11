#ifndef TRACERMANAGER_H
#define TRACERMANAGER_H

#include <QMouseEvent>
#include <QObject>
#include "mytracer.h"
#include "qcustomplot.h"

class TracerManager
{
    Q_OBJECT
public:
    static TracerManager *getTracerManagerInstance(QCustomPlot *customPlot);
    static TracerManager *getTracerManagerInstance();
    static void destroyTracerManagerInstance();
    void setTracerCustomPlot(QCustomPlot *customPlot);
    void setTracerVisible(bool visible);
public slots:
    void showTracer(QMouseEvent *event, QVector<QColor> colorVector);

private:
    //单例模式
    static TracerManager *tracerManagerInstance;
    TracerManager(QCustomPlot *customPlot);
    virtual ~TracerManager();
    QSharedPointer<myTracer> m_TracerX;
    QSharedPointer<myTracer> m_TracerY;
};

#endif // TRACERMANAGER_H
