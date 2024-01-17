#ifndef TRACERMANAGER_H
#define TRACERMANAGER_H

#include <QMouseEvent>
#include <QObject>
#include "module/mytracer.h"
#include "module/qcustomplot.h"

class TracerManager : public QObject
{
    Q_OBJECT
public:
    static TracerManager *getTracerManagerInstance(QCustomPlot *customPlot);
    static TracerManager *getTracerManagerInstance();
    static void destroyTracerManagerInstance();
    void setTracerCustomPlot(QCustomPlot *customPlot);
    void setTracerVisible(bool visible);
    bool getTracerVisible();
public slots:
    void showTracer(QMouseEvent *event, QVector<QColor> colorVector);

private:
    //单例模式
    static TracerManager *tracerManagerInstance;
    TracerManager(QCustomPlot *customPlot, QObject *parent = nullptr);
    virtual ~TracerManager();
    QSharedPointer<myTracer> m_TracerX;
    QSharedPointer<myTracer> m_TracerY;
};

#endif // TRACERMANAGER_H