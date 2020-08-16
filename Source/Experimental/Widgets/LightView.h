#pragma once

#include "Application/ImGuiLab.h"
#include "Render/Scene.h"

class LightView
{
public:
    SPtr<Scene> scene;

    void SetScene(const SPtr<Scene> &inScene)
    {
        scene = inScene;
    }

    void OnDirectional(const SPtr<DirectionalLight> &light)
    {
        Vector3 dir = light->GetDirection();
        if (ImGui::DragFloat3("Direction", &dir.x, 0.001f, -1.0f, 1.0f))
            light->SetDirection(dir);
    }

    void OnGui()
    {
        if (!ImGui::CollapsingHeader("Lights"))
            return;

        if (!scene || scene->GetLights().IsEmpty())
        {
            ImGui::Text("Light count: 0.");
            return;
        }

        auto &lights = scene->GetLights();
        ImGui::Text("Light count: %d.", lights.Count());

        for (int32 i = 0; i < lights.Count(); ++i)
        {
            auto &light = lights[i];
            auto lightType = light->GetLightType();
            String typeString = lightType == LightType::Point ? CT_TEXT("Point") : CT_TEXT("Directional");
            String stringID = String::Format(CT_TEXT("#{0}:{1}({2})"), i, light->GetName(), typeString);
            if (ImGui::TreeNode(CT_U8_CSTR(stringID)))
            {
                Color color = light->GetIntensityColor();
                float scale = light->GetIntensityScale();
                if (ImGui::ColorEdit3("Intensity color", &color.r))
                {
                    light->SetIntensityColor(color);
                }
                if (ImGui::SliderFloat("Intensity scale", &scale, 0.0f, 200.0f))
                {
                    light->SetIntensityScale(scale);
                }

                if (auto p = std::dynamic_pointer_cast<DirectionalLight>(light))
                {
                    OnDirectional(p);
                }

                ImGui::TreePop();
            }
        }
    }
};