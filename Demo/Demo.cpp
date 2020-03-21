#include "Application/Application.h"
#include "Application/ImGuiLab.h"
#include "Render/Shader.h"
#include "Render/RenderAPI.h"
#include "Render/OrthographicCamera.h"
#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "IO/FileWatcher.h"
#include "Core/Thread.h"
#include "Demo/Page1.h"

class Demon : public Logic
{
private:
    SPtr<VertexArray> vertexArray;
    SPtr<Shader> shader;
    SPtr<Texture> texture;
    OrthographicCamera camera{0.0f, 0.0f};

    UPtr<IO::FileWatcher> watcher;
    bool reloadShader = false;

    UPtr<Page> page;
    int32 pageIndex = -1;

private:
    void SetCurrentPage(int32 index)
    {
        static Array<std::function<UPtr<Page>(void)>> creators{
            []() { return Page1::Create(); }};

        if (index < 0 || index >= creators.Count())
        {
            CT_EXCEPTION(Demo, "Invalid index.");
            return;
        }

        if (index == pageIndex)
        {
            return;
        }

        pageIndex = index;
        page = creators[index]();

        auto &window = gApp->GetWindow();
        window.SetTitle(page->GetName());
    }

public:
    virtual void OnLoad() override
    {
        SetCurrentPage(0);

        auto &window = gApp->GetWindow();
        float width = window.GetWidth();
        float height = window.GetHeight();
        camera.viewportWidth = width;
        camera.viewportHeight = height;
        camera.Update();

        window.windowResizedHandler.On([this](auto &event) {
            RenderAPI::SetViewport(0, 0, event.width, event.height);

            camera.viewportWidth = (float)event.width;
            camera.viewportHeight = (float)event.height;
            camera.Update();
        });

        gImGuiLab->drawHandler.On([this]() {
            page->OnImGuiDraw();
        });

        float vertexData[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};

        uint32 indexData[] = {
            0, 1, 2,
            2, 3, 0};

        auto vertexBuffer = VertexBuffer::Create(vertexData, sizeof(vertexData));
        vertexBuffer->SetLayout({
            {CT_TEXT("position"), VertexDataType::Float3},
            {CT_TEXT("texCoord"), VertexDataType::Float2},
        });

        auto indexBuffer = IndexBuffer::Create(indexData, 6);

        vertexArray = VertexArray::Create();
        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);

        texture = Texture::Create(CT_TEXT("Assets/Textures/wheel.png"));

        shader = Shader::Create(page->GetShaderPath());

        watcher = Memory::MakeUnique<IO::FileWatcher>(
            CT_TEXT("Assets/Shaders"),
            [this](const String &path, IO::FileWatcher::FileStatus status) {
                if (status == IO::FileWatcher::FileStatus::Changed)
                {
                    reloadShader = true;
                }
            });

        std::thread watchThread([this]() {
            watcher->Start();
        });
        watchThread.detach();
    }

    virtual void OnUnload() override
    {
        watcher->Stop();
    }

    virtual void OnUpdate() override
    {
        RenderAPI::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        RenderAPI::Clear();

        if (reloadShader)
        {
            reloadShader = false;
            shader = Shader::Create(page->GetShaderPath());
        }

        shader->Bind();

        Matrix4 scaleMat = Matrix4::Scale(texture->GetWidth(), texture->GetHeight(), 1.0f);
        shader->SetMatrix4(CT_TEXT("model"), scaleMat);
        shader->SetMatrix4(CT_TEXT("view"), camera.view);
        shader->SetMatrix4(CT_TEXT("projection"), camera.projection);
        shader->SetInt(CT_TEXT("mainTex"), 0);

        texture->Bind();
        vertexArray->Bind();

        RenderAPI::DrawIndexed(vertexArray);
    }
};

Demon demon;
Logic *gLogic = &demon;