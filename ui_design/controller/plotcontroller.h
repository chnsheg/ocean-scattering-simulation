#ifndef PLOTCONTROLLER_H
#define PLOTCONTROLLER_H

/***************************控制层***************************/

#include <QObject>
#include "base/singleton.h"
#include "manager/inputdatalistmanager.h"
#include "model/pagedatagenerator.h"
#include "view/plotView.h"
#include "view/dynamicview.h"
#include "module/hoverinfowidget.h"
#include <QPixmap>

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
    void handleSwitchPageButtonClicked(int page_index, QRect area = QRect());
    void handleStoreRuntimeDataSignal(QSharedPointer<QCPGraphDataContainer> dataContainer, const int page_index, const int curve_index);
    void handleShowButtonHover(int index, const QPoint &pos);
    void handleShowButtonLeave(int index);

private:
    explicit PlotController(PlotView *_view, PageDataGenerator *_model, QObject *parent = nullptr);
    // ~PlotController();
    static PlotController *plotControllerInstance;
    PlotView *view;
    PageDataGenerator *model;
    QThread *thread;
    QMetaObject::Connection connection;
    // DynamicPage *dynamicView;
    QVector<DynamicPage *> dynamicViewVector;
    QVector<int> dynamicViewOpened; // 用来记录打开的dynamicView的index，这样即使关闭了dynamicView，也可以在对应的位置打开新的dynamicView

    QVector<HoverInfoWidget *> hoverInfoWidgetsVector;
    QVector<int> hoverInfoWidgetsOpened; // 用来记录打开的hoverInfoWidget的index，这样即使关闭了hoverInfoWidget，也可以在对应的位置打开新的hoverInfoWidget

    SINGLETON(PlotController)
};

#endif // PLOTCONTROLLER_H
