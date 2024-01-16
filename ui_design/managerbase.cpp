#include "managerbase.h"

ManagerBase::ManagerBase(QObject *parent)
    : QObject{parent}
{}

QAction *ManagerBase::getEvent(const QString &event)
{
    return d_eventActions.value(event, nullptr);
}

bool ManagerBase::sendEvent(const QString &event)
{
    QAction *action = getEvent(event);
    if (action) {
        Q_EMIT action->triggered();
        return true;
    }
    return false;
}

bool ManagerBase::sendEvent(const QString &event, const QVariant &param)
{
    QAction *action = getEvent(event);
    if (action) {
        setEventParam(event, param);
        Q_EMIT action->triggered();
        return true;
    }
    return false;
}

QVariant ManagerBase::getEventParam(const QString &event)
{
    return d_eventParams.value(event);
}

void ManagerBase::setEventParam(const QString &event, const QVariant &param)
{
    d_eventParams.insert(event, param);
}

// template<typename Func1, typename Func2, typename... Args>
// QAction *ManagerBase::addEvent(
//     const QString &event, const QString &text, Func1 item, Func2 slot, Args &&...args)
// {
//     auto action = new QAction(text);
//     setEventParam(event, QVariant::fromValue(std::forward<Args>(args)...));
//     QObject::connect(item, std::move(slot), action, &QAction::triggered);
//     // 事件触发时，调用槽函数来发送信号
//     QObject::connect(action,
//                      &QAction::triggered,
//                      [this, event](bool checked = false) { // 事件触发时，得到参数并发送信号
//                          onEventAction(event, checked, getEventParam(event));
//                      });

//     d_eventActions.insert(event, action);
//     return action;
// }
