#include "RenderVulkan/VulkanGpuFence.h"

namespace RenderCore
{

SPtr<GpuFence> GpuFence::Create()
{
    return Memory::MakeShared<VulkanGpuFence>();
}



}