#ifndef RETRIEVALWINDOW_H
#define RETRIEVALWINDOW_H

#include <QMainWindow>
#include "manager/customplotmanager.h"
#include "manager/texteditmanager.h"
#include "utils/logger.h"

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
    };

    struct RetrievalError
    {
        double temperatureError;
        double salinityError;
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
    Ui::RetrievalWindow *ui;
    QVector<MesurementError> m_mesurementError;
    QVector<RetrievalError> m_retrievalError;

public slots:
    void onShowButtonClicked();
    void onClearButtonClicked();
    void saveLineEditGroupsText();
    void onRetrievalCompleted(QVariantList *args);

signals:
    void closeRetrievalWindow();
};

#endif // RETRIEVALWINDOW_H
