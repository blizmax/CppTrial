#include "Application/Application.h"
#include "Render/Shader.h"
#include "Render/RenderAPI.h"
#include "Render/OrthographicCamera.h"
#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "IO/FileWatcher.h"
#include "Core/Thread.h"

class Demon : public Logic
{
private:
    SPtr<VertexArray> vertexArray;
    SPtr<Shader> shader;
    SPtr<Texture> texture;
    OrthographicCamera camera{0.0f, 0.0f};

    UPtr<IO::FileWatcher> watcher;
    bool reloadShader = false;

public:
    virtual void OnLoad() override
    {
        // Input &input = gApp->GetInput();
        // input.mouseMovedEventHandler.On([](InputEvent &event) {
        //     CT_LOG(Debug, event.ToString());
        // });

        auto &window = gApp->GetWindow();
        window.SetTitle(CT_TEXT("New Title"));

        //gApp->GetClipboard().SetString(CT_TEXT("XXXXXXX"));

        // window.filesDroppedEventHandler.On([](WindowEvent &event)
        // {
        //     auto &e = static_cast<FilesDroppedEvent&>(event);
        //     CT_LOG(Debug, CT_TEXT("count:{0}, path1:{1}"), e.paths.Size(), e.paths[0]);

        //     gApp->GetWindow().Flash();
        // });

        float width = window.GetWidth();
        float height = window.GetHeight();
        camera.viewportWidth = width;
        camera.viewportHeight = height;
        camera.Update();

        window.windowResizeEventHandler.On([this](WindowEvent &event) {
            auto &e = static_cast<WindowResizeEvent &>(event);
            RenderAPI::SetViewport(0, 0, e.width, e.height);

            camera.viewportWidth = (float)e.width;
            camera.viewportHeight = (float)e.height;
            camera.Update();
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

        shader = Shader::Create(CT_TEXT("Assets/Shaders/Shader1.glsl"));

        watcher = Memory::MakeUnique<IO::FileWatcher>(CT_TEXT("Assets/Shaders"),
            [this](const String &path, IO::FileWatcher::FileStatus status) {
                if (status == IO::FileWatcher::FileStatus::Changed)
                {
                    if (path.EndsWith(CT_TEXT("Shader1.glsl")))
                    {
                        reloadShader = true;
                    }
                }
            });

        std::thread watchThread([this]() {
            CT_LOG(Debug, CT_TEXT("Watch thread created."));
            watcher->Start();
            CT_LOG(Debug, CT_TEXT("Watch thread destroyed."));
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
            shader = Shader::Create(CT_TEXT("Assets/Shaders/Shader1.glsl"));
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

        //CT_LOG(Info, CT_TEXT("FPS:{0}"), gApp->GetFramesPerSecond());
    }
};

Demon demon;
Logic *gLogic = &demon;