#ifndef PLOTCONTROLLER_H
#define PLOTCONTROLLER_H

/***************************控制层***************************/

#include <QObject>
#include "base/singleton.h"
#include "manager/inputdatalistmanager.h"
#include "model/pagedatagenerator.h"
#include "view/plotView.h"

class PlotController : public QObject
{
    Q_OBJECT

public:
    // static PlotController *getPlotControllerInstance(PlotView *_view, PageDataGenerator *_model);
    // static PlotController *getPlotControllerInstance();
    // static void destroyPlotControllerInstance();
    /*friend class Singleton<PlotController>;*/ // 友元类, 使得Singleton类可以访问PlotController的私有构造函数

public slots:
    void handleStartButtonClicked(const int page_index);
    void handleDataGenerated(const QVector<QVector<double> *> *xDataVector,
                             const QVector<QVector<double> *> *yDataVector,
                             const int curve_num);
    void handleClearButtonClicked();
    void handleTracerButtonClicked();
    void handleSwitchPageButtonClicked(int page_index);

private:
    explicit PlotController(PlotView *_view, PageDataGenerator *_model, QObject *parent = nullptr);
    // ~PlotController();
    static PlotController *plotControllerInstance;
    PlotView *view;
    PageDataGenerator *model;

    SINGLETON(PlotController)
};

#endif // PLOTCONTROLLER_H
