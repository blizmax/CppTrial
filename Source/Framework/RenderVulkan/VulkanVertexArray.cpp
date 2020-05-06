#include "RenderVulkan/VulkanVertexArray.h"

namespace RenderCore
{
SPtr<VertexArray> VertexArray::Create()
{
    return Memory::MakeShared<VulkanVertexArray>();
}    
}