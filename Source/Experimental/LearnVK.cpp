#include "Application/Application.h"
#include "Application/ImGuiLab.h"
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
    SPtr<Program> program;
    SPtr<GraphicsVars> vars;

    SPtr<Scene> scene;
    SPtr<CameraController> cameraController;
    float sceneUpdateTime = 0.0f;

public:
    Renderer()
    {
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

        state->SetRasterizationState(rasterizationState);
        state->SetDepthStencilState(depthStencilState);
        state->SetBlendState(blendState);

        //vars->Root()[CT_TEXT("UB")][CT_TEXT("tint")] = Color(0.7f, 0.3f, 0.0f);
    }

    void LoadScene(const String &path)
    {
        auto &window = gApp->GetWindow();
        float width = window.GetWidth();
        float height = window.GetHeight();

        SceneImporter importer;
        auto settings = SceneImportSettings::Create();
        settings->dontLoadBones = true;
        scene = importer.Import(path, settings);

        if (scene)
        {
            sceneUpdateTime = 0.0f;

            cameraController = FirstPersonCameraController::Create(scene->GetCamera());
            cameraController->SetViewport(width, height);
            scene->SetCameraController(cameraController);
            ProgramCompileOptions options;
            options.generateDebugInfo = true;
            program = Program::Create(CT_TEXT("Assets/Shaders/Experimental/LearnVK.glsl"), scene->GetSceneDefines(), options);
            vars = GraphicsVars::Create(program);
            state->SetProgram(program);
        }
    }

    void Render(RenderContext *ctx, const SPtr<FrameBuffer> &fbo)
    {
        if (!scene)
            return;

        sceneUpdateTime += gApp->GetDeltaTime();

        state->SetFrameBuffer(fbo);
        scene->Update(ctx, sceneUpdateTime);

        SceneRenderFlags flags = SceneRender::None;
        scene->Render(ctx, state.get(), vars.get(), flags);
    }
};

class LearnVK : public Logic
{
private:
    SPtr<Renderer> renderer;
    ImGui::FileBrowser fileDialog;

public:
    virtual void Startup() override
    {
        renderer = Memory::MakeShared<Renderer>();

        gImGuiLab->drawHandler.On([this]() {
            ImGui::Begin("ModelViewer");

            ImGuiIO &io = ImGui::GetIO();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Separator();

            if (ImGui::Button("Open file"))
                fileDialog.Open();

            fileDialog.Display();

            if (fileDialog.HasSelected())
            {
                renderer->LoadScene(fileDialog.GetSelected().c_str());
                fileDialog.ClearSelected();
            }

            ImGui::End();
        });
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
        if (!renderer->scene)
            return;

        renderer->cameraController->SetViewport((float)e.width, (float)e.height);
    }

    virtual void OnKeyDown(KeyDownEvent &e) override
    {
        if (!renderer->scene)
            return;

        renderer->cameraController->OnKeyDown(e);

        if (e.key == CT_KEY_R)
        {
            renderer->scene->ResetCamera();
        }
    }

    virtual void OnKeyUp(KeyUpEvent &e) override
    {
        if (!renderer->scene)
            return;
        renderer->cameraController->OnKeyUp(e);
    }

    virtual void OnTouchDown(TouchDownEvent &e) override
    {
        if (!renderer->scene)
            return;
        renderer->cameraController->OnTouchDown(e);
    }

    virtual void OnTouchUp(TouchUpEvent &e) override
    {
        if (!renderer->scene)
            return;
        renderer->cameraController->OnTouchUp(e);
    }

    virtual void OnMouseMoved(MouseMovedEvent &e) override
    {
        if (!renderer->scene)
            return;
        renderer->cameraController->OnMouseMoved(e);
    }

    virtual void OnMouseScrolled(MouseScrolledEvent &e) override
    {
        if (!renderer->scene)
            return;
        renderer->cameraController->OnMouseScrolled(e);
    }
};

LearnVK logic;
Logic *gLogic = &logic;