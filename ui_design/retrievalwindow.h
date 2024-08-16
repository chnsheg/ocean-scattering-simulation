#ifndef RETRIEVALWINDOW_H
#define RETRIEVALWINDOW_H

#include <QMainWindow>
#include "manager/customplotmanager.h"
#include "manager/texteditmanager.h"
#include "utils/logger.h"
#include <QMap>
#include "view/dynamicview.h"

namespace Ui
{
    class RetrievalWindow;
}

class RetrievalWindow : public QMainWindow
{
    Q_OBJECT

    // 定义一个误差的结构体，包含布里渊频移、布里渊线宽、瑞利线宽、温度、盐度误差
    struct MesurementError
    {
        double BrillouinShiftError;
        double BrillouinWidthError;
        double rayleighWidthError;

        bool operator==(const MesurementError &other) const
        {
            return BrillouinShiftError == other.BrillouinShiftError &&
                   BrillouinWidthError == other.BrillouinWidthError &&
                   rayleighWidthError == other.rayleighWidthError;
        }
    };

    struct RetrievalError
    {
        double temperatureError;
        double salinityError;

        bool operator==(const RetrievalError &other) const
        {
            return temperatureError == other.temperatureError &&
                   salinityError == other.salinityError;
        }
    };

public:
    explicit RetrievalWindow(QWidget *parent = nullptr);
    ~RetrievalWindow();
    QTextEdit *getRetrievalTextEdit();
    QCustomPlot *getRetrievalCustomPlot();
    QCustomPlot *getMesurementCustomPlot();

private:
    void initWindowStyle();
    void boundButtonEvent();
    void drawRetrievalErrorScatterPlot();
    void drawMesurementErrorScatterPlot();
    void drawHistogram(QCustomPlot *customPlot, const QMap<double, int> &histogram, QString title);
    void calculateRetrievalError();
    QVector<double> *calculateAverageError();
    void calculateDepthsRetrievalError(int index, double depth, QVector<double> *retrievalData);
    QMap<double, int> calculateHistogram(const QVector<double> &data, int binCount = 10);
    QMap<double, int> calculateHistogramWithinRange(const QVector<double> &data, double minRange, double maxRange, int binCount = 10);
    void calculateTSStatistics(DynamicPage *dynamicView);
    Ui::RetrievalWindow *ui;
    // QVector<MesurementError> m_mesurementError;
    // QVector<RetrievalError> m_retrievalError;
    QMap<double, MesurementError> m_mesurementError;
    QMap<double, RetrievalError> m_retrievalError;

    QVector<DynamicPage *> dynamicViewVector;
    QVector<int> dynamicViewOpened; // 用来记录打开的dynamicView的index，这样即使关闭了dynamicView，也可以在对应的位置打开新的dynamicView
    double m_minRange;
    double m_maxRange;
    QVector<QCPItemText *> textLabels; // 用 QVector 保存文本标签的指针

public slots:
    void onShowButtonClicked();
    void onClearButtonClicked();
    void onExtendButtonClicked();
    void onStatisticButtonClicked();
    void saveLineEditGroupsText();
    void onRetrievalCompleted(QVariantList *args);

signals:
    void closeRetrievalWindow();
};

#endif // RETRIEVALWINDOW_H
