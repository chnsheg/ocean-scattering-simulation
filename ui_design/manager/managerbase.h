#ifndef MANAGERBASE_H
#define MANAGERBASE_H

#include <QAction>
#include <QMap>
#include <QObject>
#include <QString>
#include <QVariant>

class ManagerBase : public QObject
{
    Q_OBJECT
public:
    explicit ManagerBase(QObject *parent = nullptr);

public:
    QAction *getEvent(const QString &event);

    bool sendEvent(const QString &event); //用于触发事件，即内部触发
    bool sendEvent(const QString &event, const QVariant &param);

    QVariant getEventParam(const QString &event);
    void setEventParam(const QString &event, const QVariant &param);

    template<typename Func1, typename Func2, typename... Args>
    QAction *addEvent(
        const QString &event, const QString &text, Func1 item, Func2 slot, Args &&...args)
    {
        auto action = new QAction(text);
        setEventParam(event, QVariant::fromValue(std::forward<Args>(args)...));
        QObject::connect(item, std::move(slot), action, &QAction::triggered); //外部触发事件
        // 事件触发时，调用槽函数来发送信号
        QObject::connect(action,
                         &QAction::triggered,
                         [this,
                          event](int checked = false) { // check为保留参数，取自QAction::triggered
                             onEventAction(event, checked, getEventParam(event));
                         });

        d_eventActions.insert(event, action);
        return action;
    }

    //取消事件订阅
    template<typename Func1, typename Func2>
    void removeEvent(const QString &event, Func1 item, Func2 slot)
    {
        auto action = getEvent(event);
        if (action) {
            QObject::disconnect(item, std::move(slot), action, &QAction::triggered);
            delete action;
        }
        d_eventActions.remove(event);
        d_eventParams.remove(event);
    }

protected slots:
    virtual void onEventAction(const QString &event, int status, const QVariant &param)
        = 0; //统一的事件处理的接口

public:
signals:
    virtual void eventSignal(const QVariant &param); //统一的释放信号的接口，可重写

private:
    QMap<QString, QAction *> d_eventActions;
    QMap<QString, QVariant> d_eventParams;
};

#endif // MANAGERBASE_H
