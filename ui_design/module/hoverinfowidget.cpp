#include "HoverInfoWidget.h"
#include <QTimer>
#include <QDebug>

HoverInfoWidget::HoverInfoWidget(QWidget *parent)
    : QWidget(parent), draggable(false), pinned(false), resizeable(false), borderMargin(8), originalPixmap(nullptr), baseWidth(400), baseHeight(500)
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

    // 设置窗口可以增大和缩小
    // Expandint
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    // 设置窗口的最小大小
    setMinimumSize(320, 400);

    // 设置窗口的最大大小
    setMaximumSize(640, 800);

    // 设置窗口大小,但是可以通过resize()函数调整窗口大小
    setFixedSize(baseWidth, baseHeight);

    setAttribute(Qt::WA_TranslucentBackground); // Set attribute to make the widget transparent
    setMouseTracking(true);

    imageLabel = new QLabel(this);
    imageLabel->setScaledContents(true);
    imageLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    imageLabel->setAlignment(Qt::AlignCenter);

    infoListWidget = new QListWidget(this);
    infoListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    infoListWidget->setFrameStyle(QFrame::NoFrame);
    // 设置item的分割
    infoListWidget->setStyleSheet("QListWidget::item { border-bottom: 1px solid #dcdcdc; }");
    infoListWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    // 设置item的字体"Lato", Helvetica, Arial, sans-serif;
    infoListWidget->setFont(QFont("Lato", 10));
    // 设置item选中时的背景颜色以及透明度，和字体颜色
    infoListWidget->setStyleSheet("QListWidget::item:selected { background-color: #f7f7f7; color: #333; }");

    closeButton = new QPushButton("❌", this);
    closeButton->setFlat(true);
    closeButton->setFixedSize(50, 60);
    closeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(closeButton, &QPushButton::clicked, this, &HoverInfoWidget::onCloseButtonClicked);

    pinButton = new QPushButton("📌", this);
    pinButton->setFlat(true);
    pinButton->setFixedSize(50, 60);
    pinButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(pinButton, &QPushButton::clicked, this, &HoverInfoWidget::onPinButtonClicked);

    // 设置窗口大小的按键（按比例增大窗口大小）
    resizeButton = new QPushButton("➕", this);
    resizeButton->setFlat(true);
    resizeButton->setFixedSize(50, 60);
    resizeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(resizeButton, &QPushButton::clicked, this, &HoverInfoWidget::onResizeButtonClicked);

    // 窗口中间防止一个小方块状的输入框，用来显示当前窗口的大小比例
    resizeLineEdit = new QLineEdit(this);
    resizeLineEdit->setFixedSize(100, 60);
    resizeLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    resizeLineEdit->setAlignment(Qt::AlignCenter);
    resizeLineEdit->setText("100%");
    // 处理输入框的回车事件
    connect(resizeLineEdit, &QLineEdit::returnPressed, this, &HoverInfoWidget::onResizeLineEditReturnPressed);
    // 设置输入框的字体
    resizeLineEdit->setFont(QFont("Lato", 10));
    // 设置字体颜色
    resizeLineEdit->setStyleSheet("color: #333;");
    // 设置输入框的背景颜色
    resizeLineEdit->setStyleSheet("background-color: #f4f4f4;");

    // 设置窗口大小的按键（按比例减小窗口大小）
    shrinkButton = new QPushButton("➖", this);
    shrinkButton->setFlat(true);
    shrinkButton->setFixedSize(50, 60);
    shrinkButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(shrinkButton, &QPushButton::clicked, this, &HoverInfoWidget::onShrinkButtonClicked);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    // 使两个按键都在右侧
    buttonsLayout->addWidget(shrinkButton, 0, Qt::AlignLeft);
    buttonsLayout->addWidget(resizeLineEdit, 0, Qt::AlignLeft);
    buttonsLayout->addWidget(resizeButton, 0, Qt::AlignLeft);

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
    int listItemHeight = infoListWidget->sizeHintForRow(0) * (infoListWidget->count() + 1);
    infoListWidget->setFixedSize(imageWidth, listItemHeight);

    setFixedSize(imageWidth + 2 * borderMargin, closeButton->height() + imageHeight + listItemHeight + 2 * borderMargin);
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

    if (originalPixmap != nullptr)
    {
        delete originalPixmap;
    }
    this->originalPixmap = new QPixmap(pixmap);

    int imageWidth = width() - 2 * borderMargin;
    // 缩放图片
    // int imageHeight = pixmap.isNull() ? 0 : imageWidth * pixmap.height() / pixmap.width();
    // 无失真地缩放图片
    QPixmap scaledPixmap = pixmap.scaledToWidth(imageWidth, Qt::SmoothTransformation);
    int imageHeight = scaledPixmap.height();
    // imageLabel->resize(imageWidth, imageHeight);
    imageLabel->setFixedSize(imageWidth, imageHeight);
    imageLabel->setPixmap(scaledPixmap);
    adjustComponents(); // Adjust components after setting the image
}

void HoverInfoWidget::updateDisplayImage()
{
    if (originalPixmap != nullptr)
    {
        qDebug() << "updateDisplayImage:" << width();
        int imageWidth = width() - 2 * borderMargin;
        QPixmap scaledPixmap = originalPixmap->scaledToWidth(imageWidth, Qt::SmoothTransformation);
        int imageHeight = scaledPixmap.height();
        // imageLabel->resize(imageWidth, imageHeight);
        imageLabel->setFixedSize(imageWidth, imageHeight);
        imageLabel->setPixmap(scaledPixmap);
        adjustComponents(); // Adjust components after setting the image
    }
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
        QTimer::singleShot(200, [this]
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
    // 给卡片悬浮窗添加投影效果，向下和向右偏移
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 50));
    painter.drawRoundedRect(rect().translated(4, 4), 10, 10);

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

void HoverInfoWidget::onResizeButtonClicked()
{
    // 获取当前窗口的大小
    int currentWidth = this->width();
    int currentHeight = this->height();
    // 设置窗口的大小

    // 计算增大后的宽度和高度，使用浮点数相乘
    int newWidth = floor(currentWidth * 1.1);
    int newHeight = floor(currentHeight * 1.1);

    if (newWidth > baseWidth * HOVER_INFO_WIDGET_MAX_WIDTH_Ratio)
    {
        newWidth = baseWidth * HOVER_INFO_WIDGET_MAX_WIDTH_Ratio;
    }
    else if (newWidth < baseWidth * HOVER_INFO_WIDGET_MIN_WIDTH_Ratio)
    {
        newWidth = baseWidth * HOVER_INFO_WIDGET_MIN_WIDTH_Ratio;
    }

    setFixedSize(newWidth, newHeight);

    // // 设置窗口大小的比例
    resizeLineEdit->setText(QString::number((int)((double)newWidth / baseWidth * 100)) + "%");

    updateDisplayImage();
    adjustComponents();
}

void HoverInfoWidget::onShrinkButtonClicked()
{
    // 获取当前窗口的大小
    int currentWidth = this->width();
    int currentHeight = this->height();

    // 计算缩小后的宽度和高度，使用浮点数相乘
    int newWidth = floor(currentWidth * 0.9);
    int newHeight = floor(currentHeight * 0.9);

    if (newWidth < baseWidth * HOVER_INFO_WIDGET_MIN_WIDTH_Ratio)
    {
        newWidth = baseWidth * HOVER_INFO_WIDGET_MIN_WIDTH_Ratio;
    }
    else if (newWidth > baseWidth * HOVER_INFO_WIDGET_MAX_WIDTH_Ratio)
    {
        newHeight = baseHeight * HOVER_INFO_WIDGET_MAX_WIDTH_Ratio;
    }

    // 先把窗口内图片的大小缩小
    // imageLabel->resize(0, 0);

    // 设置窗口的大小
    // resize(newWidth, newHeight);
    setFixedSize(newWidth, newHeight);
    // // 设置窗口大小的比例
    resizeLineEdit->setText(QString::number((int)((double)newWidth / baseWidth * 100)) + "%");
    updateDisplayImage();
    adjustComponents();
}

void HoverInfoWidget::onResizeLineEditReturnPressed()
{
    // 获取输入框中的文本
    QString text = resizeLineEdit->text();
    // 获取当前窗口的大小
    int currentWidth = this->baseWidth;
    int currentHeight = this->baseHeight;
    // 获取输入框中的数字
    int scale = text.left(text.length() - 1).toInt();

    if (scale < HOVER_INFO_WIDGET_MIN_WIDTH_Ratio * 100)
    {
        scale = HOVER_INFO_WIDGET_MIN_WIDTH_Ratio * 100;
    }
    else if (scale > HOVER_INFO_WIDGET_MAX_WIDTH_Ratio * 100)
    {
        scale = HOVER_INFO_WIDGET_MAX_WIDTH_Ratio * 100;
    }

    qDebug() << "scale:" << scale;
    // 设置窗口的大小
    // resize(currentWidth * scale / 100, currentHeight * scale / 100);
    // 向下取整
    setFixedSize(floor(currentWidth * scale / 100), floor(currentHeight * scale / 100));
    updateDisplayImage();
    adjustComponents();
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
