#include "HoverInfoWidget.h"

HoverInfoWidget::HoverInfoWidget(QWidget *parent)
    : QWidget(parent), draggable(false), pinned(false), resizeable(false), borderMargin(8)
{
    setupUI();
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
    // buttonsLayout->addWidget(closeButton, 0, Qt::AlignLeft);
    // buttonsLayout->addStretch();
    // buttonsLayout->addWidget(pinButton, 0, Qt::AlignRight);
    // 使两个按键都在右侧
    buttonsLayout->addStretch();

    buttonsLayout->addWidget(pinButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(closeButton, 0, Qt::AlignRight);
    // 设置按键之间的间距
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    buttonsLayout->setSpacing(0);
    buttonsLayout->setMargin(0);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(buttonsLayout);
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
    int imageWidth = width() - 2 * borderMargin;
    int imageHeight = static_cast<int>(imageWidth * 0.8);
    imageLabel->setFixedSize(imageWidth, imageHeight);

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
    imageLabel->setPixmap(pixmap);
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
        // close();
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
