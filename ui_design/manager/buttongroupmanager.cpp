#include "manager/buttongroupmanager.h"

ButtonGroupsManager::ButtonGroupsManager(QVector<ButtonGroup> *_buttonGroups, QObject *parent)
    : ManagerBase(parent), buttonGroups(_buttonGroups)
{
    buttonGroups = _buttonGroups;
    initButtonStyle(1);
    initButtonStatus(1);

    for (int i = 0; i < buttonGroups->size(); i++)
    {
        addEvent(QString("start%1ButtonClicked").arg(i + 1),
                 QString("start%1ButtonClicked").arg(i + 1),
                 buttonGroups->at(i).showButton,
                 &QPushButton::clicked,
                 ButtonGroupId::showButton);
        addEvent(QString("clear%1ButtonClicked").arg(i + 1),
                 QString("clear%1ButtonClicked").arg(i + 1),
                 buttonGroups->at(i).clearButton,
                 &QPushButton::clicked,
                 ButtonGroupId::clearButton);
        addEvent(QString("tracer%1ButtonClicked").arg(i + 1),
                 QString("tracer%1ButtonClicked").arg(i + 1),
                 buttonGroups->at(i).tracerButton,
                 &QPushButton::clicked,
                 ButtonGroupId::tracerButton);
        addEvent(QString("back%1ButtonClicked").arg(i + 1),
                 QString("back%1ButtonClicked").arg(i + 1),
                 buttonGroups->at(i).backButton,
                 &QPushButton::clicked,
                 static_cast<ButtonGroupId>(static_cast<int>(ButtonGroupId::back1Button) + i));
        addEvent(QString("saveConstant%1ButtonClicked").arg(i + 1),
                 QString("saveConstant%1ButtonClicked").arg(i + 1),
                 buttonGroups->at(i).saveConstantButton,
                 &QPushButton::clicked,
                 static_cast<ButtonGroupId>(static_cast<int>(ButtonGroupId::saveConstant1Button) + i));
    }
}

ButtonGroupsManager::ButtonGroupsManager() {}

ButtonGroupsManager::~ButtonGroupsManager()
{
    // 释放内存
    if (buttonGroups != nullptr)
    {
        delete buttonGroups;
        buttonGroups = nullptr;
    }
}

// ButtonGroupsManager *ButtonGroupsManager::ButtonGroupsManagerInstance = nullptr;

// ButtonGroupsManager *ButtonGroupsManager::getButtonGroupsManagerInstance(
//     QVector<ButtonGroup> *buttonGroups)
// {
//     if (ButtonGroupsManagerInstance == nullptr)
//         ButtonGroupsManagerInstance = new ButtonGroupsManager(buttonGroups);
//     return ButtonGroupsManagerInstance;
// }

// ButtonGroupsManager *ButtonGroupsManager::getButtonGroupsManagerInstance()
// {
//     if (ButtonGroupsManagerInstance == nullptr)
//         // 在这里你可能想要抛出一个异常或者采取其他处理方式
//         // 因为没有指定 QCustomPlot 的实例，单例模式无法正常工作
//         return nullptr;
//     return ButtonGroupsManagerInstance;
// }

// void ButtonGroupsManager::destroyButtonGroupsManagerInstance()
// {
//     if (ButtonGroupsManagerInstance != nullptr) {
//         delete ButtonGroupsManagerInstance;
//         ButtonGroupsManagerInstance = nullptr;
//     }
// }

// TODO:初始化按钮样式
void ButtonGroupsManager::initButtonStyle(int index)
{
    // Set button style accordingly
    // for (int i = 0; i < buttonGroups->size(); i++)
    // {
    //     buttonGroups->at(i).showButton->setStyleSheet("QPushButton{border-image: url(:/images/Show.png);}"
    //                                                   "QPushButton:hover{border-image: url(:/images/ShowHover.png);}"
    //                                                   "QPushButton:pressed{border-image: url(:/images/ShowPressed.png);}");
    //     buttonGroups->at(i).clearButton->setStyleSheet("QPushButton{border-image: url(:/images/Clear.png);}"
    //                                                    "QPushButton:hover{border-image: url(:/images/ClearHover.png);}"
    //                                                    "QPushButton:pressed{border-image: url(:/images/ClearPressed.png);}");
    //     buttonGroups->at(i).tracerButton->setStyleSheet("QPushButton{border-image: url(:/images/Tracer.png);}"
    //                                                     "QPushButton:hover{border-image: url(:/images/TracerHover.png);}"
    //                                                     "QPushButton:pressed{border-image: url(:/images/TracerPressed.png);}");
    //     buttonGroups->at(i).backButton->setStyleSheet("QPushButton{border-image: url(:/images/Back.png);}"
    //                                                   "QPushButton:hover{border-image: url(:/images/BackHover.png);}"
    //                                                   "QPushButton:pressed{border-image: url(:/images/BackPressed.png);}");
    // }
}

void ButtonGroupsManager::initButtonStatus(int index)
{
    // Set button status accordingly
    updateButtonStatus(index, ButtonStatus(true, false, false));
    updateTracerButtonText(index, false);
}

void ButtonGroupsManager::updateButtonStatus(int index, const ButtonStatus &status)
{
    // Update button status accordingly
    buttonGroups->at(index - 1).showButton->setEnabled(status.showEnabled);
    buttonGroups->at(index - 1).clearButton->setEnabled(status.clearEnabled);
    buttonGroups->at(index - 1).tracerButton->setEnabled(status.tracerEnabled);
}

void ButtonGroupsManager::updateTracerButtonText(int index, bool isVisible)
{
    // Update tracer button accordingly
    buttonGroups->at(index - 1).tracerButton->setText(isVisible ? "隐藏光标" : "显示光标");
}

void ButtonGroupsManager::onEventAction(const QString &event, int status, const QVariant &param)
{
    // ButtonGroupId buttonGroupId = static_cast<ButtonGroupId>(param.toInt());
    ButtonGroupId buttonId = param.value<ButtonGroupId>();
    emit eventSignal(buttonId);
    // qDebug() << "ButtonGroupsManager::onEventAction \n";
    // qDebug() << "event: " << event << "status: " << status << "param: " << param;
}

// 获取buttonGroups
//  QVector<ButtonGroup> *ButtonGroupsManager::getButtonGroups()
//  {
//      return buttonGroups;
//  }

// void ButtonGroupsManager::startButtonClicked()
// {
//     // qDebug() << "startButtonClicked";
//     emit startButtonGroupClicked();
// }

// void ButtonGroupsManager::clearButtonClicked()
// {
//     // qDebug() << "clearButtonClicked";
//     emit clearButtonGroupClicked();
// }

// void ButtonGroupsManager::tracerButtonClicked()
// {
//     // qDebug() << "tracerButtonClicked";
//     emit tracerButtonGroupClicked();
// }

// void ButtonGroupsManager::back1ButtonClicked()
// {
//     emit backButtonGroupClicked(1); //第几个绘图界面的返回按键返回到第几个展示界面
// }

// void ButtonGroupsManager::back2ButtonClicked()
// {
//     emit backButtonGroupClicked(1);
// }

// void ButtonGroupsManager::back3ButtonClicked()
// {
//     emit backButtonGroupClicked(1);
// }

// void ButtonGroupsManager::back4ButtonClicked()
// {
//     emit backButtonGroupClicked(1);
// }

// void ButtonGroupsManager::back5ButtonClicked()
// {
//     emit backButtonGroupClicked(1);
// }
