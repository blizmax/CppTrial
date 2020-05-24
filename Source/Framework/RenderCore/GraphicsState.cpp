#include "RenderCore/GraphicsState.h"

namespace RenderCore
{

GraphicsState::GraphicsState()
{
    int32 maxCount = VIEWPORT_MAX_NUM;

    viewports.SetCount(maxCount);
    scissors.SetCount(maxCount);

    for(int32 i = 0; i < maxCount; ++i)
    {
        SetViewport(i, viewports[i], true);
    }
}


}