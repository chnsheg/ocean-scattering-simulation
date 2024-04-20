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

class HoverInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HoverInfoWidget(QWidget *parent = nullptr);
    ~HoverInfoWidget();

    void setInfo(const QMap<QString, QVariant> &info);
    void setDisplayImage(const QPixmap &pixmap);
    void setAnchorPoint(const QPoint &point);
    // 获取卡片窗口的锚点位置
    QPoint getAnchorPoint() const;
    void showWithEffect();
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
    QPoint anchorPoint;
    QPoint dragPosition;
    bool draggable;
    bool pinned;
    bool resizeable;
    int borderMargin;

    void setupUI();
    QPainterPath shapePath() const;

    void adjustComponents();

signals:
    void closeHoverInfoWidgetSignal();

public slots:
    void onCloseButtonClicked();
    void onPinButtonClicked();
};

#endif // HOVERINFOWIDGET_H
