#pragma once

#include "Render/RenderGraph/.Package.h"

class RenderGraphExecutor
{
public:
    void Execute();

    static SPtr<RenderGraphExecutor> Create();
};