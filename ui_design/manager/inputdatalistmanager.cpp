#include "manager/inputdatalistmanager.h"

InputDataListManager::InputDataListManager()
{
    //分配内存
    this->inputDataList = new QVector<QString>();
}

InputDataListManager::InputDataListManager(const InputDataListManager &inputDataList)
{
    if (this->inputDataList != nullptr) {
        delete this->inputDataList;
        this->inputDataList = nullptr;
    }
    this->inputDataList = inputDataList.inputDataList;
}

InputDataListManager::~InputDataListManager()
{
    //释放内存
    if (this->inputDataList != nullptr) {
        delete this->inputDataList;
        this->inputDataList = nullptr;
    }
}

QVector<QString> *InputDataListManager::getInputDataList() const
{
    return this->inputDataList;
}

void InputDataListManager::setInputDataList(const QString &inputDataList)
{
    this->inputDataList->push_back(inputDataList);
}
