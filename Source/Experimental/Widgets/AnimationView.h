#pragma once

#include "Application/ImGuiLab.h"
#include "Render/Scene.h"

class AnimationView
{
public:
    SPtr<Scene> scene;

    void SetScene(const SPtr<Scene> &inScene)
    {
        scene = inScene;
    }

    void OnGui()
    {
        if (!ImGui::CollapsingHeader("Animations"))
            return;
    }
};