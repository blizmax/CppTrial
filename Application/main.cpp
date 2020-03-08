#include "Application/Application.h"
#include "Render/Shader.h"
#include "Render/RenderAPI.h"
#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "IO/FileWatcher.h"
#include "Core/Thread.h"

Application app;
Application *gApp = &app;

class Demon : public Logic
{
private:
    SPtr<VertexArray> vertexArray;
    SPtr<Shader> shader;
    SPtr<Texture> texture;

    UPtr<IO::FileWatcher> watcher;
    bool reloadShader = false;

public:
    virtual void OnLoad() override
    {
        // Input &input = gApp->GetInput();
        // input.mouseMovedEventHandler.On([](InputEvent &event) {
        //     CT_LOG(Debug, event.ToString());
        // });

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
                    if(path.EndsWith(CT_TEXT("Shader1.glsl")))
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
        if(reloadShader)
        {
            reloadShader = false;
            shader = Shader::Create(CT_TEXT("Assets/Shaders/Shader1.glsl"));
        }

        //TODO Move to render mgr
        RenderAPI::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        RenderAPI::Clear();

        texture->Bind();
        shader->Bind();
        vertexArray->Bind();

        RenderAPI::DrawIndexed(vertexArray);
    }
};

Demon demon;
Logic *gLogic = &demon;

int main(int argc, char **argv)
{
    app.PreInit({CT_TEXT("Hello World"), 1280, 720});

    app.Init();

    app.Run();

    app.Exit();

    return 0;
}