#pragma once

#include "RenderCore/GraphicsStateObject.h"
#include "RenderCore/FrameBuffer.h"
#include "RenderCore/StateGraph.h"
#include "RenderCore/Program.h"
#include "RenderCore/ProgramVars.h"

class GraphicsState
{
public:
    GraphicsState();
    ~GraphicsState() = default;

    void SetViewport(int32 index, const Viewport &viewport, bool setScissor = true);
    void SetScissor(int32 index, const Scissor &scissor);
    void PushViewport(int32 index, const Viewport &viewport, bool setScissor = true);
    void PopViewport(int32 index, bool setScissor = true);
    void PushScissor(int32 index, const Scissor &scissor);
    void PopScissor(int32 index);
    void SetFrameBuffer(const SPtr<FrameBuffer> &fbo, bool setVp0Sc0 = true);
    void PushFrameBuffer(const SPtr<FrameBuffer> &fbo, bool setVp0Sc0 = true);
    void PopFrameBuffer(bool setVp0Sc0 = true);
    void SetVertexArray(const SPtr<VertexArray> &vao);
    void SetRasterizationState(const SPtr<RasterizationState> &state);
    void SetDepthStencilState(const SPtr<DepthStencilState> &state);
    void SetBlendState(const SPtr<BlendState> &state);
    void SetSampleMask(uint32 sampleMask);
    SPtr<GraphicsStateObject> GetGso(const GraphicsVars *vars);

    const Viewport &GetViewport(int32 index) const
    {
        return viewports[index];
    }

    const Array<Viewport> &GetViewports() const
    {
        return viewports;
    }

    const Scissor &GetScissor(int32 index) const
    {
        return scissors[index];
    }

    const Array<Scissor> &GetScissors() const
    {
        return scissors;
    }

    const SPtr<VertexArray> &GetVertexArray() const
    {
        return vertexArray;
    }

    const SPtr<FrameBuffer> &GetFrameBuffer() const
    {
        return frameBuffer;
    }

    void SetStencilRef(uint8 ref)
    {
        stencilRef = ref;
    }

    uint8 GetStencilRef() const
    {
        return stencilRef;
    }

    void SetProgram(const SPtr<Program> &prog)
    {
        program = prog;
    }

    const SPtr<Program> &GetProgram() const
    {
        return program;
    }

    const GraphicsStateObjectDesc &GetDesc() const
    {
        return desc;
    }

    const SPtr<RasterizationState> &GetRasterizationState() const
    {
        return desc.rasterizationState;
    }

    const SPtr<DepthStencilState> &GetDepthStencilState() const
    {
        return desc.depthStencilState;
    }

    const SPtr<BlendState> &GetBlendState() const
    {
        return desc.blendState;
    }

    uint32 GetSampleMask() const
    {
        return desc.sampleMask;
    }

    static SPtr<GraphicsState> Create();

private:
    struct CachedData
    {
        WPtr<ProgramKernel> programKernel;
        WPtr<RootSignature> rootSignature;
        WPtr<FrameBuffer> frameBuffer;
    };

    SPtr<VertexArray> vertexArray;
    SPtr<FrameBuffer> frameBuffer;
    SPtr<Program> program;
    Array<SPtr<FrameBuffer>> frameBufferStack;

    GraphicsStateObjectDesc desc;
    uint8 stencilRef = 0;
    Array<Viewport> viewports;
    Array<Scissor> scissors;
    Array<Array<Viewport>> viewportStacks;
    Array<Array<Scissor>> scissorStacks;

    CachedData cachedData;
    StateGraph<SPtr<GraphicsStateObject>> stateGraph;
};