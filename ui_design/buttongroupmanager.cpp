#include "buttongroupmanager.h"

ButtonGroupsManager::ButtonGroupsManager(QVector<ButtonGroup> *_buttonGroups)
{
    buttonGroups = _buttonGroups;
    initButtonStyle(1);
    initButtonStatus(1);
}

ButtonGroupsManager *ButtonGroupsManager::ButtonGroupsManagerInstance = nullptr;

ButtonGroupsManager *ButtonGroupsManager::getButtonGroupsManagerInstance(
    QVector<ButtonGroup> *buttonGroups)
{
    if (ButtonGroupsManagerInstance == nullptr)
        ButtonGroupsManagerInstance = new ButtonGroupsManager(buttonGroups);
    return ButtonGroupsManagerInstance;
}

ButtonGroupsManager *ButtonGroupsManager::getButtonGroupsManagerInstance()
{
    return ButtonGroupsManagerInstance;
}

void ButtonGroupsManager::destroyButtonGroupsManagerInstance()
{
    if (ButtonGroupsManagerInstance != nullptr)
        delete ButtonGroupsManagerInstance;
}

//TODO:初始化按钮样式
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
