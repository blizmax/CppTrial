#pragma once

#include "Application/ImGuiLab.h"
#include "Assets/AssetManager.h"

class ImageWindow
{
public:
    APtr<Texture> texture;
    ImGui::FileBrowser fileDialog;

public:
    void LoadImage(const String &path)
    {
        texture = gAssetManager->Import<Texture>(path, nullptr);
    }

    void OnGui()
    {
        ImGui::Begin("ImageWindow");

        if (ImGui::Button("Open file"))
            fileDialog.Open();

        fileDialog.Display();

        if (fileDialog.HasSelected())
        {
            LoadImage(fileDialog.GetSelected().c_str());
            fileDialog.ClearSelected();
        }

        ImGui::Separator();

        auto windowSize = ImGui::GetWindowSize();
        windowSize.y -= ImGui::GetCursorPosY();
        float width = texture ? texture->GetWidth() : windowSize.x;
        float height = texture ? texture->GetHeight() : windowSize.y;
        float aspect = texture ? (height / width) : 1.0f;
        if (windowSize.x * aspect < windowSize.y)
        {
            width = Math::Min<float>(windowSize.x * 0.8f, width);
            height = aspect * width;
        }
        else
        {
            height = Math::Min<float>(windowSize.y * 0.8f, height);
            width = height / aspect;
        }

        ImGui::Image(CT_IMGUI_TEXID(texture), { width, height });

        ImGui::End();
    }
};