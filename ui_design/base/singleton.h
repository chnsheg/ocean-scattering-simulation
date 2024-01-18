#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>
#include <QScopedPointer>

template<typename T>
class Singleton
{
public:
    template<typename... Args>
    static T *getInstance(Args &&...args);

    Singleton(const Singleton &other) = delete;
    Singleton<T> &operator=(const Singleton &other) = delete;

private:
    static QMutex mutex;
    static QScopedPointer<T> instance;
};

template<typename T>
QMutex Singleton<T>::mutex;
template<typename T>
QScopedPointer<T> Singleton<T>::instance;

template<typename T>
template<typename... Args>
T *Singleton<T>::getInstance(Args &&...args)
{
    if (instance.isNull()) {
        mutex.lock();
        if (instance.isNull()) {
            instance.reset(new T(std::forward<Args>(args)...));
        }
        mutex.unlock();
    }
    return instance.data();
}

#define SINGLETON(Class) \
private: \
    Class(); \
    ~Class(); \
    Class(const Class &other) = delete; \
    Class &operator=(const Class &other) = delete; \
    friend class Singleton<Class>; \
    friend struct QScopedPointerDeleter<Class>;

#endif // SINGLETON_H
