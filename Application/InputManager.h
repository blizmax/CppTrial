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

    virtual Input &GetInput() = 0;

    virtual void ProcessKeyTyped(int32 character);
    virtual void ProcessKeyDown(int32 key);
    virtual void ProcessKeyUp(int32 key);
    virtual void ProcessTouchDown(int32 button);
    virtual void ProcessTouchUp(int32 button);
    virtual void ProcessMouseMoved(int32 posX, int32 posY);
    virtual void ProcessMouseScrolled(int32 amount);

protected:
    int32 TranslateKey(int32 key)
    {
        if (key < 0 || key > CT_KEY_LAST)
            return CT_KEY_UNKNOWN;
        return keycodeTable[key];
    }

    int32 keycodeTable[512];
};

extern InputManager *gInputManager;