#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>
#include "mainwindow.h"
#include <QLayout>

class HoverButton : public QPushButton
{
    Q_OBJECT
public:
    explicit HoverButton(QWidget *parent = nullptr)
    {
        // 设置子类的大小和位置与父类完全相同
        // setGeometry(parent->geometry());
        // 设置子类的父对象为父类

        // 通过parent设置其中子类与父类完全贴合
        // parent->layout()->setSpacing(0);
        // parent->layout()->setContentsMargins(0, 0, 0, 0);

        // setParent(parent);

        // // 使按键大小随着父类的大小变化而变化
        // setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // qDebug() << "parent_size:" << parent->size();

        // // 设置子类的文本、图标、字体等与父类相同
        // setText(parent->text());
        // // setIcon(parent->icon());
        // // setIconSize(parent->iconSize());
        // setFont(parent->font());
        // // 设置字体大小
        // QFont font;
        // font.setPointSize(12);
        // setFont(font);

        // setContentsMargins(0, 0, 0, 0);

        // // 设置子类的鼠标跟踪以便能够捕获鼠标事件
        // setMouseTracking(true);
        // qDebug() << "HoverButton constructor";

        // 将子类添加到父类的布局中
        // QLayout *layout = parent->layout();
        // if (layout)
        // {
        //     layout->addWidget(this);
        // }
        // else
        // {
        //     qWarning() << "Parent does not have a layout!";
        // }

        // 设置子类的鼠标跟踪以便能够捕获鼠标事件
        setMouseTracking(true);
        // qDebug() << "HoverButton constructor";
    }

public:
    // 重写enterEvent函数
    void enterEvent(QEvent *event) override
    {
        QPushButton::enterEvent(event);
        emit hoverSignal(event);
        qDebug() << "HoverButton hover";
    }

    // 重写leaveEvent函数
    void leaveEvent(QEvent *event) override
    {
        QPushButton::leaveEvent(event);
        emit leaveSignal(event);
        qDebug() << "HoverButton leave";
    }

    // 重写点击事件
    void mousePressEvent(QMouseEvent *event) override
    {
        QPushButton::mousePressEvent(event);
        // emit clicked(event);
        qDebug() << "HoverButton clicked";
    }

signals:
    // 将按钮的悬停事件发送出去
    void hoverSignal(QEvent *event);
    void leaveSignal(QEvent *event);
};

#endif // HOVERBUTTON_H
