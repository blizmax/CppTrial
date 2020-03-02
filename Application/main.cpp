#include "Application/Application.h"
#include "Render/Shader.h"
#include "Render/RenderAPI.h"
#include "Render/VertexArray.h"

Application app;
Application *gApp = &app;

class Demon : public Logic
{
private:
    SPtr<VertexArray> vertexArray;
    SPtr<Shader> shader;
public:
    virtual void OnLoad() override
    {
        float vertexData[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        uint32 indexData[] = {
            0, 1, 2
        };

        auto vertexBuffer = VertexBuffer::Create(vertexData, sizeof(vertexData));
        vertexBuffer->SetLayout({
            {CT_TEXT("position"), VertexDataType::Float3}
        });

        auto indexBuffer = IndexBuffer::Create(indexData, 3);

        vertexArray = VertexArray::Create();
        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);

        shader = Shader::Create(CT_TEXT("Assets/Shaders/Shader1.glsl"));

        CT_LOG(Info, CT_TEXT("Logic loaded."));
    }

    virtual void OnUnload() override
    {
    }

    virtual void OnUpdate() override
    {
        //CT_LOG(Debug, CT_TEXT("Logic update."));


        //TODO Move to render mgr
        RenderAPI::SetClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        RenderAPI::Clear();

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