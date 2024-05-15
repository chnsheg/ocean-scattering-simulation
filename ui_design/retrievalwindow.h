#ifndef RETRIEVALWINDOW_H
#define RETRIEVALWINDOW_H

#include <QMainWindow>
#include "manager/customplotmanager.h"
#include "manager/texteditmanager.h"
#include "utils/logger.h"
#include <QMap>

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
    void calculateRetrievalError();
    void calculateDepthsRetrievalError(int index, double depth, QVector<double> *retrievalData);
    Ui::RetrievalWindow *ui;
    // QVector<MesurementError> m_mesurementError;
    // QVector<RetrievalError> m_retrievalError;
    QMap<double, MesurementError> m_mesurementError;
    QMap<double, RetrievalError> m_retrievalError;

public slots:
    void onShowButtonClicked();
    void onClearButtonClicked();
    void onExtendButtonClicked();
    void saveLineEditGroupsText();
    void onRetrievalCompleted(QVariantList *args);

signals:
    void closeRetrievalWindow();
};

#endif // RETRIEVALWINDOW_H
