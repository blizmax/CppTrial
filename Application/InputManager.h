#pragma once

#include "Application/.Package.h"
#include "Application/Input.h"
#include "Core/Delegate.h"
#include "Utils/Module.h"

class InputManager : public Module
{
public:
    InputManager() = default;
    virtual ~InputManager() = default;

    virtual void OnLoad() override;
    virtual void OnUnload() override;
    virtual void OnUpdate() override;

    virtual String GetName() const override
    {
        return CT_TEXT("InputManager");
    }

    virtual void ProcessKeyTyped(char32 character);
    virtual void ProcessKeyDown(int32 key);
    virtual void ProcessKeyUp(int32 key);
    virtual void ProcessTouchDown(int32 button);
    virtual void ProcessTouchUp(int32 button);
    virtual void ProcessMouseMoved(int32 posX, int32 posY);
    virtual void ProcessMouseScrolled(int32 amount);

private:
    int32 TranslateKey(int32 key)
    {
        if (key < 0 || key > CT_KEY_LAST)
            return CT_KEY_UNKNOWN;
        return keycodeTable[key];
    }

public:
    Delegate<void(char32)> keyTypedHandler;
    Delegate<void(int32)> keyDownHandler;
    Delegate<void(int32)> keyUpHandler;
    Delegate<void(int32)> touchDownHandler;
    Delegate<void(int32)> touchUpHandler;
    Delegate<void(int32)> mouseMovedHandler;
    Delegate<void(int32)> mouseScrolledHandler;

protected:
    int32 keycodeTable[512];
};

extern InputManager *gInputManager;