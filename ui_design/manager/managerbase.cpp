#include "manager/managerbase.h"

/*解释什么时候才需要继承此基类：
当你在对某一组控件进行集中管理时，不需要在其中加入业务代码，而是
只需要将改组控件的所有事件全部编码之后,由统一的信号接口传递给上层，
并在上层中解析编码后的事件，进行业务处理。这样做的好处是，将一个
manager的所有事件集中抛出又在上层集中处理，实现了业务逻辑与界面逻辑的分离。

需要注意的是：
1.继承此类后，需要实现onEventAction函数，用于统一接收事件；
2.在构造函数中，需要调用addEvent函数，将事件添加到事件列表中；
3.在析构函数中，需要调用removeEvent函数，将事件从事件列表中移除；
4.在需要发送事件的地方，调用sendEvent函数，将事件发送出去；
*/

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
