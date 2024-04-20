#include "HoverInfoWidget.h"
#include <QTimer>
#include <QDebug>

HoverInfoWidget::HoverInfoWidget(QWidget *parent)
    : QWidget(parent), draggable(false), pinned(false), resizeable(false), borderMargin(8) // 8
{
    setupUI();
}

HoverInfoWidget::~HoverInfoWidget()
{
    delete imageLabel;
    delete infoListWidget;
    delete closeButton;
    delete pinButton;

    imageLabel = nullptr;
    infoListWidget = nullptr;
    closeButton = nullptr;
    pinButton = nullptr;

    emit closeHoverInfoWidgetSignal();
}

void HoverInfoWidget::setupUI()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip); // Set window flags to make it a tooltip
    // 设置窗口大小,但是可以通过resize()函数调整窗口大小
    setFixedSize(300, 450);

    setAttribute(Qt::WA_TranslucentBackground); // Set attribute to make the widget transparent
    setMouseTracking(true);

    imageLabel = new QLabel(this);
    imageLabel->setScaledContents(true);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageLabel->setAlignment(Qt::AlignCenter);

    infoListWidget = new QListWidget(this);
    infoListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    infoListWidget->setFrameStyle(QFrame::NoFrame);

    closeButton = new QPushButton("❌", this);
    closeButton->setFlat(true);
    closeButton->setFixedSize(30, 40);
    closeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(closeButton, &QPushButton::clicked, this, &HoverInfoWidget::onCloseButtonClicked);

    pinButton = new QPushButton("📌", this);
    pinButton->setFlat(true);
    pinButton->setFixedSize(30, 40);
    pinButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(pinButton, &QPushButton::clicked, this, &HoverInfoWidget::onPinButtonClicked);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    // 使两个按键都在右侧
    buttonsLayout->addStretch();

    buttonsLayout->addWidget(pinButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(closeButton, 0, Qt::AlignRight);
    // 设置按键之间的间距
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    buttonsLayout->setSpacing(0);
    buttonsLayout->setMargin(0);
    // 设置背景颜色

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("color: #f7f7f7; border-width: 2px; border-style: solid;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(line);
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(infoListWidget);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(borderMargin);
    setLayout(mainLayout);

    setStyleSheet("QPushButton { border: none; background: none; } "
                  "HoverInfoWidget { border: 1px solid #dcdcdc; border-radius: 10px; background-color: white; }");
}

void HoverInfoWidget::adjustComponents()
{
    // 获取图片的宽度和高度
    int imageWidth = width() - 2 * borderMargin;
    int imageHeight = imageLabel->pixmap() ? imageWidth * imageLabel->pixmap()->height() / imageLabel->pixmap()->width() : 0;
    int listItemHeight = infoListWidget->sizeHintForRow(0) * infoListWidget->count();
    infoListWidget->setFixedSize(imageWidth, listItemHeight);

    resize(imageWidth + 2 * borderMargin, closeButton->height() + imageHeight + listItemHeight + 2 * borderMargin);
}

void HoverInfoWidget::setInfo(const QMap<QString, QVariant> &info)
{
    infoListWidget->clear();
    for (auto it = info.constBegin(); it != info.constEnd(); ++it)
    {
        QListWidgetItem *item = new QListWidgetItem(it.key() + ": " + it.value().toString());
        infoListWidget->addItem(item);
    }
    adjustComponents(); // Adjust components after adding items to the list widget
}

void HoverInfoWidget::setDisplayImage(const QPixmap &pixmap)
{
    if (pixmap.isNull())
    {
        imageLabel->clear();
        adjustComponents(); // Adjust components after setting the image
        return;
    }
    int imageWidth = width() - 2 * borderMargin;
    // 缩放图片
    // int imageHeight = pixmap.isNull() ? 0 : imageWidth * pixmap.height() / pixmap.width();
    // 无失真地缩放图片
    QPixmap scaledPixmap = pixmap.scaledToWidth(imageWidth, Qt::SmoothTransformation);
    int imageHeight = scaledPixmap.height();
    imageLabel->resize(imageWidth, imageHeight);
    imageLabel->setPixmap(scaledPixmap);
    adjustComponents(); // Adjust components after setting the image
}

void HoverInfoWidget::setAnchorPoint(const QPoint &point)
{
    anchorPoint = point;
}

void HoverInfoWidget::showWithEffect()
{
    // Implement any show effect here if desired
    show();
}

void HoverInfoWidget::hideWithEffect()
{
    // qDebug() << "hideWithEffect";
    // 判断鼠标是否在窗口内
    if (pinned || rect().contains(mapFromGlobal(QCursor::pos())))
    {
        return;
    }
    else
    {
        // qDebug() << "hideWithEffect closing...";
        // 如果在5s内鼠标不在窗口内，则关闭窗口
        QTimer::singleShot(500, [this]
                           {
        if (pinned || rect().contains(mapFromGlobal(QCursor::pos())))
        {
            return;
        }
        else
        {
            close();
        } });
    }
}

void HoverInfoWidget::closeNow()
{
    if (pinned)
    {
        return;
    }
    close();
}

int HoverInfoWidget::getHoverInfoWidgetWidth()
{
    return this->width();
}

int HoverInfoWidget::getHoverInfoWidgetHeight()
{
    return this->height();
}

void HoverInfoWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path = shapePath();
    painter.fillPath(path, Qt::white);
    painter.drawPath(path);
}

QPainterPath HoverInfoWidget::shapePath() const
{
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);

    QPoint tailTip = mapFromGlobal(anchorPoint);
    QPoint tailBase = QPoint(tailTip.x(), tailTip.y() + 20);
    path.moveTo(tailTip);
    path.lineTo(tailBase.x() - 10, tailBase.y());
    path.lineTo(tailBase.x() + 10, tailBase.y());
    path.lineTo(tailTip);
    return path;
}

// QPainterPath HoverInfoWidget::drawCurvePath() const
// {
//     QPainterPath path;
//     // anchorPoint是窗口外部的锚点，即窗口的锚点在窗口外部

//     path.moveTo(anchorPoint); // 移动到锚点
//     // 绘制从外部锚点anchorPoint到窗口左上角的曲线
//     QPoint controlPoint1 = QPoint(anchorPoint.x() + 50, anchorPoint.y() - 50);
//     QPoint controlPoint2 = QPoint(this->x() + 50, this->y() + 50);

// path.cubicTo(controlPoint1, controlPoint2, QPoint(this->x(), this->y()));
//     return path;
// }

QPoint HoverInfoWidget::getAnchorPoint() const
{
    QPoint globalAnchorPoint = mapToGlobal(anchorPoint);
    return globalAnchorPoint;
}

void HoverInfoWidget::enterEvent(QEvent *event)
{
    if (!pinned)
    {
        // Implement logic to stop hide effect if there's any
    }
}

void HoverInfoWidget::leaveEvent(QEvent *event)
{
    if (!pinned)
    {
        close(); // Close the widget when the mouse leaves，这时，hoverinfoWidget对象会被销毁
    }
}

void HoverInfoWidget::onCloseButtonClicked()
{
    close();
}

void HoverInfoWidget::onPinButtonClicked()
{
    pinned = !pinned;
    pinButton->setText(pinned ? "📍" : "📌");
}

void HoverInfoWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        draggable = true;
    }
}

void HoverInfoWidget::mouseReleaseEvent(QMouseEvent *event)
{
    draggable = false;
}

void HoverInfoWidget::mouseMoveEvent(QMouseEvent *event)
{
    // if (draggable && (event->buttons() & Qt::LeftButton))
    //{
    //     move(event->globalPos() - dragPosition);
    // }

    if (draggable && (event->buttons() & Qt::LeftButton))
    {
        move(event->globalPos() - dragPosition);
    }
}

void HoverInfoWidget::resizeEvent(QResizeEvent *event)
{
    adjustComponents(); // Re-adjust components on resize
}
