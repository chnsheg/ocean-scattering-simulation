#include "buttongroup.h"

ButtonGroups::ButtonGroups(QVector<ButtonGroup> *_buttonGroups)
{
    buttonGroups = _buttonGroups;
}

ButtonGroups *ButtonGroups::ButtonGroupsInstance = nullptr;

ButtonGroups *ButtonGroups::getButtonGroupsInstance(QVector<ButtonGroup> *buttonGroups)
{
    if (ButtonGroupsInstance == nullptr)
        ButtonGroupsInstance = new ButtonGroups(buttonGroups);
    return ButtonGroupsInstance;
}

ButtonGroups *ButtonGroups::getButtonGroupsInstance()
{
    return ButtonGroupsInstance;
}

void ButtonGroups::destroyButtonGroupsInstance()
{
    if (ButtonGroupsInstance != nullptr)
        delete ButtonGroupsInstance;
}

void ButtonGroups::updateButtonStatus(int index, const ButtonStatus &status)
{
    // Update button status accordingly
    buttonGroups->at(index - 1).showButton->setEnabled(status.showEnabled);
    buttonGroups->at(index - 1).clearButton->setEnabled(status.clearEnabled);
    buttonGroups->at(index - 1).tracerButton->setEnabled(status.tracerEnabled);
}

void ButtonGroups::updateTracerButtonText(int index, bool isVisible)
{
    // Update tracer button accordingly
    buttonGroups->at(index - 1).tracerButton->setText(isVisible ? "隐藏光标" : "显示光标");
}
