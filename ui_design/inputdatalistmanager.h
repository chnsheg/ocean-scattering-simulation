#ifndef INPUTDATALISTMANAGER_H
#define INPUTDATALISTMANAGER_H

#include <QString>
#include <QVector>

class InputDataListManager
{
public:
    InputDataListManager();
    InputDataListManager(const InputDataListManager &inputDataList);
    ~InputDataListManager();
    QVector<QString> *getInputDataList() const;
    void setInputDataList(const QString &inputDataList);

private:
    QVector<QString> *inputDataList;
};

#endif // INPUTDATALISTMANAGER_H
