#pragma once

#include "Demo/Page.h"

class Page2 : public Page
{
public:
    float progress = 0.0f;

    virtual String GetName() override
    {
        return CT_TEXT("Slider");
    }

    virtual void OnImGuiDraw() override
    {
        ImGui::SliderFloat("Progress", &progress, 0.0f, 1.0f);
    }

    virtual void OnShaderUpdate(SPtr<Shader> &shader) override
    {
        shader->SetFloat(CT_TEXT("Progress"), progress);
    }

    static UPtr<Page> Create()
    {
        return Memory::MakeUnique<Page2>();
    }
};