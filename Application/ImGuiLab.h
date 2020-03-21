#pragma once

#include "Application/Application.h"
#include "Render/Shader.h"
#include "Render/Texture.h"

#include "imgui.h"


class ImGuiLab : public Module
{
public:
    Delegate<void(void)> drawHandler;

    virtual void OnLoad() override;
    virtual void OnUnload() override;
    virtual void OnUpdate() override;

    virtual String GetName() const override
    {
        return CT_TEXT("ImGuiLab");
    }

private:
    void Begin();
    void End();

    void BindPlatform();
    void BindRenderer();   
    void UnbindPlatform();
    void UnbindRenderer(); 

    void SetupRenderState(ImDrawData *drawData, uint32 width, uint32 height);
    void RenderDrawData(ImDrawData *drawData);

private:
    Input::KeyTypedEventDelegate::Handle KeyTypedHandle;
    Input::KeyDownEventDelegate::Handle keyDownHandle;
    Input::KeyUpEventDelegate::Handle keyUpHandle;
    Input::TouchDownEventDelegate::Handle touchDownHandle;
    Input::TouchUpEventDelegate::Handle touchUpHandle;
    Input::MouseMovedEventDelegate::Handle mouseMovedHandle;
    Input::MouseScrolledEventDelegate::Handle mouseScrolledHandle;

    SPtr<Shader> shader;
    SPtr<Texture> texture;
};

extern ImGuiLab *gImGuiLab;
