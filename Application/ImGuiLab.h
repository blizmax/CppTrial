#pragma once

#include "Application/Application.h"
#include "Render/Shader.h"
#include "Render/Texture.h"

#include "imgui.h"


class ImGuiLab : public Module
{
public:
    Delegate<void(void)> drawEventHandler;

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
    Input::InputEventHandler::Handle KeyTypedHandle;
    Input::InputEventHandler::Handle keyUpHandle;
    Input::InputEventHandler::Handle keyDownHandle;
    Input::InputEventHandler::Handle touchUpHandle;
    Input::InputEventHandler::Handle touchDownHandle;
    Input::InputEventHandler::Handle mouseMovedHandle;
    Input::InputEventHandler::Handle mouseScrolledHandle;

    SPtr<Shader> shader;
    SPtr<Texture> texture;
};

extern ImGuiLab *gImGuiLab;
