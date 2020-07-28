#pragma once

#include "Application/Application.h"
#include "RenderCore/RenderAPI.h"
#include <imgui.h>
#include <imfilebrowser.h>

class ImGuiLab : public Module
{
public:
    Delegate<void(void)> drawHandler;

    virtual void Startup() override;
    virtual void Shutdown() override;
    virtual void Tick() override;

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

    void CreateVertexArray(ImDrawData *drawData);
    void SetupRenderState(ImDrawData *drawData, uint32 width, uint32 height);
    void RenderDrawData(ImDrawData *drawData);

private:
    SPtr<VertexLayout> vertexLayout;
    SPtr<VertexArray> vertexArray;
    SPtr<Texture> texture;
    SPtr<Sampler> sampler;
    SPtr<Program> program;
    SPtr<GraphicsVars> programVars;
    SPtr<GraphicsState> graphicsState;
};

extern ImGuiLab *gImGuiLab;
