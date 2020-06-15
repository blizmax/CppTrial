#include "Core/String.h"
#include "IO/FileHandle.h"
#include "RenderCore/RenderAPI.h"
#include "Application/Application.h"
#include "Render/RenderManager.h"

using namespace RenderCore;

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

    SPtr<Program> CreateProgram()
    {
        ProgramDesc desc;
        IO::FileHandle vertSrcFile(CT_TEXT("Assets/Shaders/LearnVK/shader.vert"));
        IO::FileHandle fragSrcFile(CT_TEXT("Assets/Shaders/LearnVK/shader.frag"));
        desc.shaderDescs.Add({ShaderType::Vertex, vertSrcFile.ReadString()});
        desc.shaderDescs.Add({ShaderType::Pixel, fragSrcFile.ReadString()});
        return Program::Create(desc);
    }

public:
    Renderer()
    {
        program = CreateProgram();
        vars = GraphicsVars::Create(program);
        vars->Root()[CT_TEXT("UB")][CT_TEXT("v")] = 1.0f;

        // {
        //     ProgramDesc desc;
        //     IO::FileHandle fragSrcFile(CT_TEXT("Assets/Shaders/LearnVK/reflection.frag"));
        //     desc.shaderDescs.Add({ShaderType::Pixel, fragSrcFile.ReadString()});
        //     reflectionProgram = Program::Create(desc);
        // }

        state = GraphicsState::Create();

        auto vertexBufferLayout = VertexBufferLayout::Create({
            {CT_TEXT("VertexPosition"), ResourceFormat::RG32Float},
            {CT_TEXT("VertexColor"), ResourceFormat::RGB32Float}
        });
        auto vertexLayout = VertexLayout::Create();
        vertexLayout->AddBufferLayout(vertexBufferLayout);

        float vertices[] = {
            0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };
        auto vbo = Buffer::Create(sizeof(vertices), ResourceBind::Vertex, CpuAccess::None, vertices);

        uint32 indices[] = {
            0, 1, 2
        };
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
            depthStencilState = DepthStencilState::Create(desc);
        }
        {
            BlendStateDesc desc;
            desc.attachments.SetCount(1);
            blendState = BlendState::Create(desc);
        }

        state->SetVertexArray(vao);
        state->SetRasterizationState(rasterizationState);
        state->SetDepthStencilState(depthStencilState);
        state->SetBlendState(blendState);
    }

    void Render(RenderContext *ctx, const SPtr<FrameBuffer> &fbo)
    {
        state->SetFrameBuffer(fbo);
        state->SetProgram(program);

        //ctx->Draw(state.get(), nullptr, 3, 0);
        ctx->DrawIndexed(state.get(), vars.get(), 3, 0, 0);
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
};

LearnVK logic;
Logic *gLogic = &logic;