#include "Application/Application.h"
#include "Application/ThreadManager.h"
#include "Application/ImGuiLab.h"
#include "IO/FileWatcher.h"
#include "Core/Thread.h"
#include "Render/OrthographicCameraController.h"
#include "Render/ImageLoader.h"
#include "RenderCore/RenderAPI.h"
#include "Render/RenderManager.h"
#include "Demos/ShaderToy/Page1.h"
#include "Demos/ShaderToy/Page2.h"
#include "Demos/ShaderToy/Page3.h"
#include "Demos/ShaderToy/Page4.h"
#include "Demos/ShaderToy/Page5.h"
#include "Demos/ShaderToy/Page6.h"
#include "Demos/ShaderToy/Page7.h"
#include "Demos/ShaderToy/Page8.h"

class ShaderToy : public Logic
{
private:
    struct ConstantBufferData
    {
        Matrix4 model;
        Matrix4 view;
        Matrix4 projection;
        float time;
    };

    SPtr<GraphicsState> state;
    SPtr<VertexArray> vao;
    SPtr<RasterizationState> rasterizationState;
    SPtr<DepthStencilState> depthStencilState;
    SPtr<BlendState> blendState;
    SPtr<Texture> texture;
    SPtr<Sampler> sampler;
    SPtr<Program> program;
    SPtr<GraphicsVars> vars;

    OrthographicCameraController cameraController;

    UPtr<IO::FileWatcher> watcher;
    std::atomic_bool reloadShader = false;

    UPtr<Page> page;
    int32 pageIndex = -1;

private:
    void SetCurrentPage(int32 index)
    {
        static Array<std::function<UPtr<Page>(void)>> creators{
            []() { return Page1::Create(); },
            []() { return Page2::Create(); },
            []() { return Page3::Create(); },
            []() { return Page4::Create(); },
            []() { return Page5::Create(); },
            []() { return Page6::Create(); },
            []() { return Page7::Create(); },
            []() { return Page8::Create(); }};

        if (index < 0 || index >= creators.Count())
        {
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

        reloadShader = true;
    }

public:
    virtual void Startup() override
    {
        SetCurrentPage(0);

        auto &window = gApp->GetWindow();
        auto &input = window.GetInput();
        float width = window.GetWidth();
        float height = window.GetHeight();

        auto camera = OrthographicCamera::Create(width, height);
        cameraController.SetCamera(camera);

        input.touchDownHandler.On([this](auto &event) {
            cameraController.OnTouchDown(event);
        });
        input.touchUpHandler.On([this](auto &event) {
            cameraController.OnTouchUp(event);
        });
        input.mouseMovedHandler.On([this](auto &event) {
            cameraController.OnMouseMoved(event);
        });
        input.mouseScrolledHandler.On([this](auto &event) {
            cameraController.OnMouseScrolled(event);
        });
        input.keyDownHandler.On([this](auto &event) {
            cameraController.OnKeyDown(event);
        });
        input.keyUpHandler.On([this](auto &event) {
            cameraController.OnKeyUp(event);
        });

        window.windowResizedHandler.On([this](auto &event) {
            cameraController.OnWindowResized(event);
        });

        gImGuiLab->drawHandler.On([this]() {
            ImGui::Begin("ShaderToy");
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Current page: %d", pageIndex);
            ImGui::SameLine();

            float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
            ImGui::PushButtonRepeat(true);
            if (ImGui::ArrowButton("##left", ImGuiDir_Left))
            {
                SetCurrentPage(pageIndex - 1);
            }
            ImGui::SameLine(0.0f, spacing);
            if (ImGui::ArrowButton("##right", ImGuiDir_Right))
            {
                SetCurrentPage(pageIndex + 1);
            }
            ImGui::PopButtonRepeat();

            page->OnImGuiDraw();
            ImGui::End();
        });


        state = GraphicsState::Create();

        auto vertexBufferLayout = VertexBufferLayout::Create({{CT_TEXT("VertexPosition"), ResourceFormat::RGB32Float},
                                                              {CT_TEXT("VertexUV"), ResourceFormat::RG32Float}});
        auto vertexLayout = VertexLayout::Create();
        vertexLayout->AddBufferLayout(vertexBufferLayout);

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};
        auto vbo = Buffer::Create(sizeof(vertices), ResourceBind::Vertex, CpuAccess::None, vertices);

        uint32 indices[] = {
            0, 1, 2, 2, 3, 0};
        auto ibo = Buffer::Create(sizeof(indices), ResourceBind::Index, CpuAccess::None, indices);

        vao = VertexArray::Create();
        vao->SetVertexLayout(vertexLayout);
        vao->AddVertexBuffer(vbo);
        vao->SetIndexBuffer(ibo);

        {
            RasterizationStateDesc desc;
            rasterizationState = RasterizationState::Create(desc);
        }
        {
            DepthStencilStateDesc desc;
            desc.depthReadEnabled = false;
            desc.depthWriteEnabled = false;
            depthStencilState = DepthStencilState::Create(desc);
        }
        {
            BlendStateDesc desc;
            desc.attachments.SetCount(1);
            desc.attachments[0].enabled = true;
            blendState = BlendState::Create(desc);
        }

        state->SetVertexArray(vao);
        state->SetRasterizationState(rasterizationState);
        state->SetDepthStencilState(depthStencilState);
        state->SetBlendState(blendState);

        {
            sampler = Sampler::Create(SamplerDesc());
            int32 w, h, c;
            void *data = ImageLoader::Load(CT_TEXT("Assets/Textures/test.png"), w, h, c);
            CT_CHECK(data);
            texture = Texture::Create2D(w, h, ResourceFormat::RGBA8Unorm, 1, 1, data);
            ImageLoader::Free(data);
        }

        watcher = Memory::MakeUnique<IO::FileWatcher>(
            CT_TEXT("Assets/Shaders"),
            [this](const String &path, IO::FileWatcher::FileStatus status) {
                if (status == IO::FileWatcher::FileStatus::Changed)
                {
                    reloadShader = true;
                }
            });

        gThreadManager->RunThread(CT_TEXT("ShaderToy watcher"), [this]() 
        {
            watcher->Start();
        });

        for(int32 i = 0; i < 1000; ++i)
        {
            gThreadManager->RunAsync([i](){
                //String str = String::Format(CT_TEXT("Thread id: {0}, i:{1}"), Thread::GetCurrentThreadID(), i);
                CT_LOG(Debug, CT_TEXT("Thread id: {0}, i:{1}"), Thread::GetCurrentThreadID(), i);
            });
        }
    }

    virtual void Shutdown() override
    {
        watcher->Stop();
    }

    virtual void Tick() override
    {
        cameraController.Update();
        auto camera = cameraController.GetCamera();

        if (reloadShader)
        {
            reloadShader = false;

            program = Program::Create(String::Format(CT_TEXT("{0}Shader{1}.glsl"), page->GetShaderDirectory(), pageIndex + 1));
            vars = GraphicsVars::Create(program);
            state->SetProgram(program);

            vars->Root()[CT_TEXT("Sampler")] = sampler;
            vars->Root()[CT_TEXT("Texture")] = texture;
        }

        page->OnShaderUpdate(vars);

        float factor = 3.0f;
        //Matrix4 rotateMat = Matrix4::Rotate(0.0f, 0.0f, 30.0f * Math::DEG_TO_RAD);
        Matrix4 rotateMat = Matrix4();
        Matrix4 scaleMat = Matrix4::Scale(texture->GetWidth() * factor, texture->GetHeight() * factor, 1.0f);
             
        static float totalTime = 0.0f;
        totalTime += gApp->GetDeltaTime();

        ConstantBufferData cbuffer;
        cbuffer.model = scaleMat * rotateMat;
        cbuffer.view = camera->view;
        cbuffer.projection = camera->projection;
        cbuffer.time = totalTime;
        vars->Root()[CT_TEXT("GBlock")].SetBlob(cbuffer);
        
        auto ctx = gRenderManager->GetRenderContext();
        state->SetFrameBuffer(gRenderManager->GetTargetFrameBuffer());
        ctx->DrawIndexed(state.get(), vars.get(), 6, 0, 0);
    }
};

ShaderToy demon;
Logic *gLogic = &demon;