#include "Core/String.h"
#include "IO/FileHandle.h"
#include "RenderCore/RenderAPI.h"
#include "Application/Application.h"
#include "Render/RenderManager.h"
#include "Render/Importers/TextureImporter.h"
#include "Render/Importers/SceneImporter.h"
#include "Render/CameraController.h"
#include "Render/MeshGenerator.h"

class Renderer
{
public:
    SPtr<CameraController> cameraController;

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

    void LoadSkull()
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

    void LoadModel()
    {
        MeshGenerator gen;
        //MeshData meshData = gen.CreateBox(1.0f, 1.0f, 1.0f, 1);
        MeshData meshData = gen.CreateSphere(1.0f, 3);

        vertices.SetCount(meshData.vertices.Count());
        for (int32 i = 0; i < meshData.vertices.Count(); ++i)
        {
            vertices[i].position = meshData.vertices[i].position;
            vertices[i].normal = meshData.vertices[i].normal;
        }
        indices = meshData.indices;
    }

public:
    Renderer()
    {
        LoadModel();

        auto &window = gApp->GetWindow();
        float width = window.GetWidth();
        float height = window.GetHeight();
        auto camera = Camera::Create();
        auto controller = OrbiterCameraController::Create(camera);
        controller->SetModelParams(Vector3(), 10.0f, 15.0f);
        controller->SetViewport(width, height);
        cameraController = controller;

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
        cameraController->Update();
        Matrix4 mvp = cameraController->GetCamera()->GetViewProjection();

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

    virtual void OnWindowResized(WindowResizedEvent &e) override
    {
        renderer->cameraController->SetViewport((float)e.width, (float)e.height);
    }

    virtual void OnKeyDown(KeyDownEvent &e) override
    {
        renderer->cameraController->OnKeyDown(e);
    }

    virtual void OnKeyUp(KeyUpEvent &e) override
    {
        renderer->cameraController->OnKeyUp(e);
    }

    virtual void OnTouchDown(TouchDownEvent &e) override
    {
        renderer->cameraController->OnTouchDown(e);
    }

    virtual void OnTouchUp(TouchUpEvent &e) override
    {
        renderer->cameraController->OnTouchUp(e);
    }

    virtual void OnMouseMoved(MouseMovedEvent &e) override
    {
        renderer->cameraController->OnMouseMoved(e);
    }

    virtual void OnMouseScrolled(MouseScrolledEvent &e) override
    {
        renderer->cameraController->OnMouseScrolled(e);
    }
};

ModelLoader logic;
Logic *gLogic = &logic;