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
    SPtr<Texture> texture;
    SPtr<Sampler> sampler;


    SPtr<Program> program;
    SPtr<GraphicsVars> vars;
    SPtr<Program> reflectionProgram;

    SPtr<Program> CreateProgram()
    {
        // ProgramDesc desc;
        // IO::FileHandle vertSrcFile(CT_TEXT("Assets/Shaders/LearnVK/shader.vert"));
        // IO::FileHandle fragSrcFile(CT_TEXT("Assets/Shaders/LearnVK/shader.frag"));
        // desc.shaderDescs.Add({ShaderType::Vertex, vertSrcFile.ReadString()});
        // desc.shaderDescs.Add({ShaderType::Pixel, fragSrcFile.ReadString()});
        // return Program::Create(desc);
        return Program::Create(CT_TEXT("Assets/Shaders/Experimental/LearnVK.glsl"));
    }

public:
    Renderer()
    {
        program = CreateProgram();
        vars = GraphicsVars::Create(program);

        // {
        //     ProgramDesc desc;
        //     IO::FileHandle fragSrcFile(CT_TEXT("Assets/Shaders/LearnVK/reflection.frag"));
        //     desc.shaderDescs.Add({ShaderType::Pixel, fragSrcFile.ReadString()});
        //     reflectionProgram = Program::Create(desc);
        // }

        state = GraphicsState::Create();

        auto vertexBufferLayout = VertexBufferLayout::Create({
            {CT_TEXT("VertexPosition"), ResourceFormat::RGB32Float},
            {CT_TEXT("VertexUV"), ResourceFormat::RG32Float}
        });
        auto vertexLayout = VertexLayout::Create();
        vertexLayout->AddBufferLayout(vertexBufferLayout);

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };
        auto vbo = Buffer::Create(sizeof(vertices), ResourceBind::Vertex, CpuAccess::None, vertices);

        uint32 indices[] = {
            0, 1, 2, 2, 3, 0
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

        state->SetProgram(program);
        state->SetVertexArray(vao);
        state->SetRasterizationState(rasterizationState);
        state->SetDepthStencilState(depthStencilState);
        state->SetBlendState(blendState);

        {
            //sampler = Sampler::Create(SamplerDesc());
            TextureImporter importer;
            texture = importer.Import(CT_TEXT("Assets/Textures/test.png"), nullptr);
        }

        vars->Root()[CT_TEXT("UB")][CT_TEXT("tint")] = Color(0.7f, 0.3f, 0.0f);
        //vars->Root()[CT_TEXT("mainSampler")] = sampler;
        vars->Root()[CT_TEXT("mainTexture")] = texture;
    }

    void Render(RenderContext *ctx, const SPtr<FrameBuffer> &fbo)
    {
        state->SetFrameBuffer(fbo);

        ctx->DrawIndexed(state.get(), vars.get(), 6, 0, 0);
    }
};

class LearnVK : public Logic
{
private:
    SPtr<Renderer> renderer;

public:
    virtual void Startup() override
    {
        // SceneImporter importer;
        // auto scene = importer.Import(CT_TEXT("Assets/Models/viking_room/viking_room.obj"), nullptr);

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