#include "RenderCore/GraphicsState.h"

SPtr<GraphicsState> GraphicsState::Create()
{
    return Memory::MakeShared<GraphicsState>();
}

GraphicsState::GraphicsState()
{
    int32 maxCount = VIEWPORT_MAX_NUM;

    viewports.SetCount(maxCount);
    scissors.SetCount(maxCount);
    viewportStacks.SetCount(maxCount);
    scissorStacks.SetCount(maxCount);

    for (int32 i = 0; i < maxCount; ++i)
    {
        SetViewport(i, viewports[i], true);
    }
}

void GraphicsState::SetViewport(int32 index, const Viewport &viewport, bool setScissor)
{
    viewports[index] = viewport;

    if (setScissor)
    {
        Scissor scissor = { (int32)viewport.x, (int32)viewport.y, (uint32)viewport.width, (uint32)viewport.height };
        SetScissor(index, scissor);
    }
}

void GraphicsState::SetScissor(int32 index, const Scissor &scissor)
{
    scissors[index] = scissor;
}

void GraphicsState::PushViewport(int32 index, const Viewport &viewport, bool setScissor)
{
    viewportStacks[index].Add(viewports[index]);
    SetViewport(index, viewport, setScissor);
}

void GraphicsState::PopViewport(int32 index, bool setScissor)
{
    if (viewportStacks[index].IsEmpty())
    {
        CT_LOG(Error, CT_TEXT("Pop viewport failed, stack is empty."));
        return;
    }

    SetViewport(index, viewportStacks[index].Last(), setScissor);
    viewportStacks[index].RemoveLast();
}

void GraphicsState::PushScissor(int32 index, const Scissor &scissor)
{
    scissorStacks[index].Add(scissors[index]);
    SetScissor(index, scissor);
}

void GraphicsState::PopScissor(int32 index)
{
    if (scissorStacks[index].IsEmpty())
    {
        CT_LOG(Error, CT_TEXT("Pop scissor failed, stack is empty."));
        return;
    }

    SetScissor(index, scissorStacks[index].Last());
    scissorStacks[index].RemoveLast();
}

void GraphicsState::SetFrameBuffer(const SPtr<FrameBuffer> &fbo, bool setVp0Sc0)
{
    frameBuffer = fbo;

    if (setVp0Sc0 && fbo)
    {
        int32 w = fbo->GetWidth();
        int32 h = fbo->GetHeight();
        Viewport viewport = { 0.0f, 0.0f, (float)w, (float)h, 0.0f, 1.0f };
        SetViewport(0, viewport, true);
    }
}

void GraphicsState::PushFrameBuffer(const SPtr<FrameBuffer> &fbo, bool setVp0Sc0)
{
    frameBufferStack.Add(frameBuffer);
    SetFrameBuffer(fbo, setVp0Sc0);
}

void GraphicsState::PopFrameBuffer(bool setVp0Sc0)
{
    if (frameBufferStack.IsEmpty())
    {
        CT_LOG(Error, CT_TEXT("Pop frame buffer failed, stack is empty."));
        return;
    }

    SetFrameBuffer(frameBufferStack.Last(), setVp0Sc0);
    frameBufferStack.RemoveLast();
}

void GraphicsState::SetVertexArray(const SPtr<VertexArray> &vao)
{
    if (vertexArray != vao)
    {
        vertexArray = vao;
        stateGraph.Walk(vao ? (void *)vao->GetVertexLayout().get() : nullptr);
    }
}

void GraphicsState::SetRasterizationState(const SPtr<RasterizationState> &state)
{
    if (desc.rasterizationState != state)
    {
        desc.rasterizationState = state;
        stateGraph.Walk((void *)state.get());
    }
}

void GraphicsState::SetDepthStencilState(const SPtr<DepthStencilState> &state)
{
    if (desc.depthStencilState != state)
    {
        desc.depthStencilState = state;
        stateGraph.Walk((void *)state.get());
    }
}

void GraphicsState::SetBlendState(const SPtr<BlendState> &state)
{
    if (desc.blendState != state)
    {
        desc.blendState = state;
        stateGraph.Walk((void *)state.get());
    }
}

void GraphicsState::SetSampleMask(uint32 sampleMask)
{
    if (desc.sampleMask != sampleMask)
    {
        desc.sampleMask = sampleMask;
        stateGraph.Walk((void *)(uint64)sampleMask);
    }
}

SPtr<GraphicsStateObject> GraphicsState::GetGso(const GraphicsVars *vars)
{
    CT_CHECK(program != nullptr);
    CT_CHECK(frameBuffer != nullptr);
    CT_CHECK(vertexArray != nullptr);

    auto programKernel = program->GetKernel();
    if (programKernel != cachedData.programKernel.lock())
    {
        cachedData.programKernel = programKernel;
        stateGraph.Walk((void *)programKernel.get());
    }

    auto rootSignature = programKernel->GetRootSignature();
    if (rootSignature != cachedData.rootSignature.lock())
    {
        cachedData.rootSignature = rootSignature;
        stateGraph.Walk((void *)rootSignature.get());
    }

    if (frameBuffer != cachedData.frameBuffer.lock())
    {
        cachedData.frameBuffer = frameBuffer;
        stateGraph.Walk((void *)frameBuffer.get());
    }

    auto gso = stateGraph.GetCurrentNodeData();
    if (gso == nullptr)
    {
        desc.programKernel = program->GetKernel();
        desc.frameBufferDesc = frameBuffer->GetDesc();
        desc.vertexLayout = vertexArray->GetVertexLayout();
        desc.topology = vertexArray->GetTopology();
        desc.rootSignature = rootSignature;

        auto Predicate = [this](const SPtr<GraphicsStateObject> &o) {
            return o && (o->GetDesc() == desc);
        };
        if (stateGraph.MatchesByPredicate(Predicate))
        {
            gso = stateGraph.GetCurrentNodeData();
        }
        else
        {
            gso = GraphicsStateObject::Create(desc);
            stateGraph.SetCurrentNodeData(gso);
        }
    }

    if (gso)
    {
        stateGraph.MarkAsRecent();
    }

    return gso;
}