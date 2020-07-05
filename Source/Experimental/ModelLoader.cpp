#include "Core/String.h"
#include "IO/FileHandle.h"
#include "RenderCore/RenderAPI.h"
#include "Application/Application.h"
#include "Render/RenderManager.h"
#include "Render/Importers/TextureImporter.h"
#include "Render/Importers/SceneImporter.h"
#include "Math/Color.h"

class Renderer
{
private:
    SPtr<GraphicsState> state;
    SPtr<VertexArray> vao;
    SPtr<RasterizationState> rasterizationState;
    SPtr<DepthStencilState> depthStencilState;
    SPtr<BlendState> blendState;

    SPtr<Program> program;
    SPtr<GraphicsVars> vars;
    SPtr<Program> reflectionProgram;


    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
    };

    Array<Vertex> vertices;
    Array<uint32> indices;

    SPtr<Program> CreateProgram()
    {
        return Program::Create(CT_TEXT("Assets/Shaders/Experimental/ModelLoader.glsl"));
    }

    void LoadModel()
    {
        CT_LOG(Debug, CT_TEXT("Load model begin."));
        int64 ts = Time::MilliTime();

        IO::FileHandle file(CT_TEXT("Assets/Models/skull/skull.txt"));
        auto ifs = file.Read();

        int32 vertexCount = 0;
        int32 triangleCount = 0;
        std::string ignore;

        ifs >> ignore >> vertexCount;
        ifs >> ignore >> triangleCount;
        ifs >> ignore >> ignore >> ignore >> ignore;

        vertices.SetCount(vertexCount);
        for (int32 i = 0; i < vertexCount; ++i)
        {
            ifs >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;
            ifs >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
        }

        ifs >> ignore;
        ifs >> ignore;
        ifs >> ignore;

        indices.SetCount(triangleCount * 3);
        for (int32 i = 0; i < triangleCount; ++i)
        {
            ifs >> indices[i * 3] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
        }

        float t = (Time::MilliTime() - ts) / 1000.0f; 
        CT_LOG(Debug, CT_TEXT("Load model end, total used time:{0}, vertexCount:{1}, triangleCount:{2}"), t, vertexCount, triangleCount);  
    }

public:
    Renderer()
    {
        LoadModel();

        program = CreateProgram();
        vars = GraphicsVars::Create(program);

        state = GraphicsState::Create();

        auto vertexBufferLayout = VertexBufferLayout::Create({
            {CT_TEXT("VertexPosition"), ResourceFormat::RGB32Float},
            {CT_TEXT("VertexNormal"), ResourceFormat::RGB32Float}
        });
        auto vertexLayout = VertexLayout::Create();
        vertexLayout->AddBufferLayout(vertexBufferLayout);

        auto vbo = Buffer::Create(sizeof(Vertex) * vertices.Count(), ResourceBind::Vertex, CpuAccess::None, vertices.GetData());
        auto ibo = Buffer::Create(sizeof(uint32) * indices.Count(), ResourceBind::Index, CpuAccess::None, indices.GetData());

        vao = VertexArray::Create();
        vao->SetVertexLayout(vertexLayout);
        vao->AddVertexBuffer(vbo);
        vao->SetIndexBuffer(ibo);

        {
            RasterizationStateDesc desc;
            desc.frontCCW = true;
            desc.polygonMode = PolygonMode::Wireframe;
            rasterizationState = RasterizationState::Create(desc);
        }
        {
            DepthStencilStateDesc desc;
            desc.depthReadEnabled = true;
            desc.depthWriteEnabled = true;
            depthStencilState = DepthStencilState::Create(desc);
        }
        {
            BlendStateDesc desc;
            desc.attachments.SetCount(1);
            desc.attachments[0].enabled = true;
            blendState = BlendState::Create(desc);
        }

        state->SetProgram(program);
        state->SetVertexArray(vao);
        state->SetRasterizationState(rasterizationState);
        state->SetDepthStencilState(depthStencilState);
        state->SetBlendState(blendState);

    }

    void Render(RenderContext *ctx, const SPtr<FrameBuffer> &fbo)
    {
        static float totalTime = 0.0f;
        auto &window = gApp->GetWindow();
        auto width = window.GetWidth();
        auto height = window.GetHeight();
        totalTime += gApp->GetDeltaTime();

        const float radius = 15.0f;
        Matrix4 view = Matrix4::LookAt(Vector3(Math::Cos(totalTime) * radius, 15.0f, Math::Sin(totalTime) * radius), Vector3(), Vector3::Y);
        Matrix4 proj = Matrix4::Projection(45.0f, width / height, 0.1f, 100.0f);
        Matrix4 mvp = proj * view;
        vars->Root()[CT_TEXT("UB")][CT_TEXT("mvp")] = mvp;

        state->SetFrameBuffer(fbo);

        ctx->DrawIndexed(state.get(), vars.get(), indices.Count(), 0, 0);
    }
};

class ModelLoader : public Logic
{
private:
    SPtr<Renderer> renderer;

public:
    virtual void Startup() override
    {

        renderer = Memory::MakeShared<Renderer>();
    }

    virtual void Shutdown() override
    {
        renderer.reset();
    }

    virtual void Tick() override
    {
        renderer->Render(gRenderManager->GetRenderContext(), gRenderManager->GetTargetFrameBuffer());
    }
};

ModelLoader logic;
Logic *gLogic = &logic;