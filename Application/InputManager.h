#pragma once

#include "Application/.Package.h"
#include "Core/Delegate.h"
#include "Utils/Module.h"

class InputManager : public Module
{
public:
    InputManager() = default;
    virtual ~InputManager() = default;

    virtual void OnLoad() override {};
    virtual void OnUnload() override {};
    virtual void OnUpdate() override {};

    virtual String GetName() const override
    {
        return CT_TEXT("InputManager");
    }

    virtual void OnKeyTyped(char32 character) {};
    virtual void OnKeyDown(int32 key, int32 repeat) {};
    virtual void OnKeyUp(int32 key) {};
    virtual void OnTouchDown(int32 button) {};
    virtual void OnTouchUp(int32 button) {};
    // virtual void OnTouchDragged(int32 button) {};
    virtual void OnMouseMoved(int32 posX, int32 posY) {};
    virtual void OnMouseScrolled(int32 amount) {};
};

extern InputManager *gInputManager;