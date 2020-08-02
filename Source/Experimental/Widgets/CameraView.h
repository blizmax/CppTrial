#pragma once

#include "Application/ImGuiLab.h"
#include "Render/Scene.h"

class CameraView
{
public:
    SPtr<Scene> scene;

    void SetScene(const SPtr<Scene> &inScene)
    {
        scene = inScene;
    }

    void OnGui()
    {
        if (!ImGui::CollapsingHeader("Camera"))
            return;

        auto &camera = scene ? scene->GetCamera() : nullptr;
        if (!camera)
        {
            ImGui::Text("No camera.");
            return;
        }

        Vector3 position = camera->GetPosition();
        if (ImGui::DragFloat3("Position", &position.x, 0.001f, -FLT_MAX, FLT_MAX))
            camera->SetPosition(position);

        Vector3 target = camera->GetTarget();
        if (ImGui::DragFloat3("Target", &target.x, 0.001f, -FLT_MAX, FLT_MAX))
            camera->SetTarget(target);

        Vector3 up = camera->GetUp();
        if (ImGui::DragFloat3("Up", &up.x, 0.001f, -FLT_MAX, FLT_MAX))
            camera->SetUp(up);
    }
};