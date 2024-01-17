#ifndef PAGEPLOT_H
#define PAGEPLOT_H

/***************************控制层***************************/

#include <QObject>
#include "manager/inputdatalistmanager.h"
#include "model/pagedatagenerator.h"
#include "view/viewcontroller.h"

class PagePlot : public QObject
{
    Q_OBJECT
public:
    static PagePlot *getPagePlotInstance(ViewController *_view, PageDataGenerator *_model);
    static PagePlot *getPagePlotInstance();
    static void destroyPagePlotInstance();

public slots:
    void handleStartButtonClicked(const InputDataListManager *inputDataList);
    void handleDataGenerated(const QVector<QVector<double> *> *xDataVector,
                             const QVector<QVector<double> *> *yDataVector,
                             const int curve_num);
    void handleClearButtonClicked();
    void handleTracerButtonClicked();
    void handleSwitchPageButtonClicked(int page_index);

private:
    explicit PagePlot(ViewController *_view, PageDataGenerator *_model, QObject *parent = nullptr);
    ~PagePlot();
    static PagePlot *pagePlotInstance;
    ViewController *view;
    PageDataGenerator *model;
};

#endif // PAGEPLOT_H
