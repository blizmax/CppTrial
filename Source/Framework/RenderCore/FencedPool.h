#pragma once

#include "RenderCore/GpuFence.h"
#include "Core/List.h"

namespace RenderCore
{

template <typename T>
class FencedPool
{
public:
    using CreateFunc = std::function<T()>;

    FencedPool(const SPtr<GpuFence> &fence, const CreateFunc &func)
        : fence(fence), createFunc(func)
    {
        activeObject = createFunc();
    }

    T NewObject()
    {
        QueueData data;
        data.obj = activeObject;
        data.val = fence->GetCpuValue();
        queue.Add(data);

        data = queue.First();
        if(data.val <= fence->GetGpuValue())
        {
            queue.RemoveFirst();
        }
        else
        {
            data.obj = createFunc();
        }

        activeObject = data.obj;
        return activeObject;
    }

    static SPtr<FencedPool> Create(const SPtr<GpuFence> &fence, const CreateFunc &func)
    {
        return Memory::MakeShared<FencedPool>(fence, func);
    }

private:
    struct QueueData
    {
        T obj;
        uint64 val;
    };

    SPtr<GpuFence> fence;
    CreateFunc createFunc;
    List<QueueData> queue;
    T activeObject;
};

}