#include "Application/Application.h"
#include "Core/String.h"
#include "IO/FileHandle.h"
#include "Math/Color.h"
#include "Render/Importers/SceneImporter.h"
#include "Render/Importers/TextureImporter.h"
#include "Render/RenderManager.h"
#include "RenderCore/RenderAPI.h"

class Renderer
{
public:
    SPtr<GraphicsState> state;
    SPtr<RasterizationState> rasterizationState;
    SPtr<DepthStencilState> depthStencilState;
    SPtr<BlendState> blendState;
    SPtr<Sampler> sampler;

    SPtr<Scene> scene;
    SPtr<Program> program;
    SPtr<GraphicsVars> vars;

    SPtr<CameraController> cameraController;

public:
    Renderer()
    {
        auto &window = gApp->GetWindow();
        float width = window.GetWidth();
        float height = window.GetHeight();

        SceneImporter importer;
        scene = importer.Import(CT_TEXT("Assets/Models/viking_room/viking_room.obj"), nullptr);
        //scene = importer.Import(CT_TEXT("Assets/Models/nanosuit_reflection/nanosuit.obj"), nullptr);
        //scene = importer.Import(CT_TEXT("Assets/Models/bee/Bee.glb"), nullptr);
        cameraController = FirstPersonCameraController::Create(scene->GetCamera());
        cameraController->SetViewport(width, height);
        scene->SetCameraController(cameraController);

        ProgramCompileOptions options;
        options.generateDebugInfo = true;
        program = Program::Create(CT_TEXT("Assets/Shaders/Experimental/LearnVK.glsl"), scene->GetSceneDefines(), options);
        vars = GraphicsVars::Create(program);
        state = GraphicsState::Create();

        {
            RasterizationStateDesc desc;
            desc.cullMode = CullMode::None;
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
        state->SetRasterizationState(rasterizationState);
        state->SetDepthStencilState(depthStencilState);
        state->SetBlendState(blendState);

        //vars->Root()[CT_TEXT("UB")][CT_TEXT("tint")] = Color(0.7f, 0.3f, 0.0f);
    }

    void Render(RenderContext *ctx, const SPtr<FrameBuffer> &fbo)
    {
        static float time = 0.0f;
        time += gApp->GetDeltaTime();

        state->SetFrameBuffer(fbo);
        scene->Update(ctx, time);
        scene->Render(ctx, state.get(), vars.get(), SceneRender::CustomRasterizationState);
    }
};

class LearnVK : public Logic
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

        if (e.key == CT_KEY_R)
        {
            renderer->scene->ResetCamera();
        }
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

LearnVK logic;
Logic *gLogic = &logic;