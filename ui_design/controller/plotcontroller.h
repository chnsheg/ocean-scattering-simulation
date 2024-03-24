#ifndef PLOTCONTROLLER_H
#define PLOTCONTROLLER_H

/***************************控制层***************************/

#include <QObject>
#include "base/singleton.h"
#include "manager/inputdatalistmanager.h"
#include "model/pagedatagenerator.h"
#include "view/plotView.h"
#include "view/dynamicview.h"

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
    void handleDataGenerated(QVector<QVector<double> *> *xDataVector,
                             QVector<QVector<double> *> *yDataVector,
                             int curve_num);
    void handleImportConstantCompleted(const int page_index, const QVector<QVector<QVector<double> *> *> *xDataVector,
                                       const QVector<QVector<QVector<double> *> *> *yDataVector);
    void handleClearButtonClicked();
    void handleTracerButtonClicked();
    void handleDynamicButtonClicked(int index);
    void handleSaveConstantButtonClicked(int index, int save_type);
    void handleImportConstantButtonClicked(int index, int import_type);
    void handleSwitchPageButtonClicked(int page_index);
    void handleStoreRuntimeDataSignal(QSharedPointer<QCPGraphDataContainer> dataContainer, const int page_index, const int curve_index);

private:
    explicit PlotController(PlotView *_view, PageDataGenerator *_model, QObject *parent = nullptr);
    // ~PlotController();
    static PlotController *plotControllerInstance;
    PlotView *view;
    PageDataGenerator *model;
    // QThread *thread;
    // DynamicPage *dynamicView;

    SINGLETON(PlotController)
};

#endif // PLOTCONTROLLER_H
