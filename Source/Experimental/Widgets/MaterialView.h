#pragma once

#include "Application/ImGuiLab.h"
#include "Render/Scene.h"

class MaterialView
{
public:
    SPtr<Scene> scene;

    void SetScene(const SPtr<Scene> &inScene)
    {
        scene = inScene;
    }

    void OnGui()
    {
        if (!ImGui::CollapsingHeader("Materials"))
            return;

        if (!scene || scene->GetMaterials().IsEmpty())
        {
            ImGui::Text("Material count: 0.");
            return;
        }

        auto &materials = scene->GetMaterials();
        ImGui::Text("Material count: %d.", materials.Count());

        ImVec2 imgSize = { 200, 200 };

        for (int32 i = 0; i < materials.Count(); ++i)
        {
            auto &mat = materials[i];
            String stringID = String::Format(CT_TEXT("#{0}:{1}"), i, mat->GetName());
            if (ImGui::TreeNode(CT_U8_CSTR(stringID)))
            {
                if (auto texture = mat->GetBaseColorTexture())
                {
                    ImGui::Text("Base color");
                    ImGui::Image(CT_IMGUI_TEXID(texture), imgSize);
                }
                else
                {
                    auto color = mat->GetBaseColor();
                    if (ImGui::ColorEdit4("Base color", &color.r))
                    {
                        mat->SetBaseColor(color);
                    }
                }

                if (auto texture = mat->GetSpecularTexture())
                {
                    ImGui::Text("Specular");
                    ImGui::Image(CT_IMGUI_TEXID(texture), imgSize);
                }
                else
                {
                    auto color = mat->GetSpecularColor();
                    if (ImGui::ColorEdit4("Specular", &color.r))
                    {
                        mat->SetSpecularColor(color);
                    }
                }

                if (auto texture = mat->GetEmissiveTexture())
                {
                    ImGui::Text("Emissive");
                    ImGui::Image(CT_IMGUI_TEXID(texture), imgSize);
                }
                else
                {
                    auto color = mat->GetEmissiveColor();
                    if (ImGui::ColorEdit4("Emissive", &color.r))
                    {
                        mat->SetEmissiveColor(color);
                    }
                }

                if (auto texture = mat->GetOcclusionTexture())
                {
                    ImGui::Text("Occlusion");
                    ImGui::Image(CT_IMGUI_TEXID(texture), imgSize);
                }

                if (auto texture = mat->GetNormalTexture())
                {
                    ImGui::Text("Normal");
                    ImGui::Image(CT_IMGUI_TEXID(texture), imgSize);
                }


                bool doubleSided = mat->IsDoubleSided();
                if (ImGui::Checkbox("Double-sided", &doubleSided))
                    mat->SetDoubleSided(doubleSided);

                ImGui::TreePop();
            }
        }
    }
};