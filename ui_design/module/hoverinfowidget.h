#ifndef HOVERINFOWIDGET_H
#define HOVERINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QMap>
#include <QVariant>
#include <QListWidget>
#include <QResizeEvent>
#include <QPainterPath>
#include <QLineEdit>

#define HOVER_INFO_WIDGET_MIN_WIDTH_Ratio 0.8
#define HOVER_INFO_WIDGET_MAX_WIDTH_Ratio 2.0

class HoverInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HoverInfoWidget(QWidget *parent = nullptr);
    ~HoverInfoWidget();

    void setInfo(const QMap<QString, QVariant> &info);
    void setDisplayImage(const QPixmap &pixmap);
    void updateDisplayImage();
    void setAnchorPoint(const QPoint &point);
    // 获取卡片窗口的锚点位置
    QPoint getAnchorPoint() const;
    void showWithEffect();
    void hideWithEffect();
    void closeNow();

    int getHoverInfoWidgetWidth();
    int getHoverInfoWidgetHeight();

    bool getPinStatus();

    void resizeEvent(QResizeEvent *event) override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QLabel *imageLabel;
    QListWidget *infoListWidget;
    QPushButton *closeButton;
    QPushButton *pinButton;
    QPushButton *resizeButton;
    QPushButton *shrinkButton;
    QLineEdit *resizeLineEdit;
    // 用于存储原图像，以便在resize时使用
    QPixmap *originalPixmap;
    QPoint anchorPoint;
    QPoint dragPosition;
    bool draggable;
    bool pinned;
    bool resizeable;
    int borderMargin;
    int baseWidth;
    int baseHeight;

    void setupUI();
    QPainterPath shapePath() const;

    void adjustComponents();

signals:
    void closeHoverInfoWidgetSignal();

public slots:
    void onCloseButtonClicked();
    void onPinButtonClicked();
    void onResizeButtonClicked();
    void onShrinkButtonClicked();
    void onResizeLineEditReturnPressed();
};

#endif // HOVERINFOWIDGET_H
