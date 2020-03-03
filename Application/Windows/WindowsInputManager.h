#pragma once

#include "Application/InputManager.h"

class WindowsInputManager : public InputManager
{
public:
    virtual void OnLoad() override;

protected:
    using InputManager::keycodeTable;
};