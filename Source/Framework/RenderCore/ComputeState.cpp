#include "RenderCore/ComputeState.h"

SPtr<ComputeState> ComputeState::Create()
{
    return Memory::MakeShared<ComputeState>();
}

ComputeState::ComputeState()
{
}

SPtr<ComputeStateObject> ComputeState::GetCso(const ComputeVars *vars)
{
    CT_CHECK(program != nullptr);

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

    auto cso = stateGraph.GetCurrentNodeData();
    if (cso == nullptr)
    {
        desc.programKernel = program->GetKernel();
        desc.rootSignature = rootSignature;

        auto Predicate = [this](const SPtr<ComputeStateObject> &o) {
            return o && (o->GetDesc() == desc);
        };
        if (stateGraph.MatchesByPredicate(Predicate))
        {
            cso = stateGraph.GetCurrentNodeData();
        }
        else
        {
            cso = ComputeStateObject::Create(desc);
            stateGraph.SetCurrentNodeData(gso);
        }
    }

    if (cso)
    {
        stateGraph.MarkAsRecent();
    }

    return cso;
}