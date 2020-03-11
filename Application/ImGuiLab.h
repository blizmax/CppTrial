#pragma once

#include "Application/Application.h"
#include "Render/Shader.h"
#include "Render/Texture.h"

#include "imgui.h"


class ImGuiLab : public Module
{
public:    
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

private:
    Input::InputEventHandler::Handle keyUpHandle;
    Input::InputEventHandler::Handle keyDownHandle;

    SPtr<Shader> shader;
    SPtr<Texture> texture;
};

extern ImGuiLab *gImGuiLab;
