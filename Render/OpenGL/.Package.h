#pragma once

#include "Render/.Package.h"

#include <glad/glad.h>

namespace Render
{
uint32 GetGLDataType(VertexDataType dataType);
uint32 GetGLPixelFormat(PixelFormat format);
uint32 GetGLPixelDataType(PixelFormat format);
uint32 GetGLPixelInternalFormat(PixelFormat format);
uint32 GetGLTextureWrap(TextureWrap wrap);
uint32 GetGLTextureFilter(TextureFilter filter);
uint32 GetGLMinMipFilter(TextureFilter min, TextureFilter mip);
uint32 GetGLUsage(GpuBufferUsage usage);
uint32 GetGLBlendFactor(BlendFactor factor);
uint32 GetGLBlendOperation(BlendOperation operation);
uint32 GetGLStencilOperation(StencilOperation operation);
uint32 GetCompareOperation(CompareOperation operation);

}