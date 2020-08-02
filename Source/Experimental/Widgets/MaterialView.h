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

        for (int32 i = 0; i < materials.Count(); ++i)
        {
            auto &mat = materials[i];
            String stringID = String::Format(CT_TEXT("{0}-{1}"), i, mat->GetName());
            if (ImGui::TreeNode(CT_U8_CSTR(stringID)))
            {
                bool doubleSided = mat->IsDoubleSided();
                if (ImGui::Checkbox("Double-sided", &doubleSided))
                    mat->SetDoubleSided(doubleSided);

                ImGui::TreePop();
            }
        }
    }
};