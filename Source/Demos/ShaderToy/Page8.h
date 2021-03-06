#pragma once

#include "Demos/ShaderToy/Page.h"
#include "Math/Color.h"

class Page8 : public TPage<Page8>
{
public:
    Color skyColor{ 0.0f, 0.0f, 0.35f };
    Color moonColor{ 1.0f, 1.0f, 0.8f };
    Color cloudColor{ 0.3f, 0.3f, 0.6f, 0.2f };

    virtual String GetName() override
    {
        return CT_TEXT("Night2D");
    }

    virtual void OnImGuiDraw() override
    {
        ImGui::ColorEdit3("SkyColor", &skyColor[0]);
        ImGui::ColorEdit3("MoonColor", &moonColor[0]);
        ImGui::ColorEdit3("CloudColor", &cloudColor[0]);
    }

    virtual void OnShaderUpdate(SPtr<GraphicsVars> &vars) override
    {
        auto rootVar = vars->Root();
        rootVar[CT_TEXT("SkyColor")] = skyColor;
        rootVar[CT_TEXT("MoonColor")] = moonColor;
        rootVar[CT_TEXT("CloudColor")] = cloudColor;
    }
};