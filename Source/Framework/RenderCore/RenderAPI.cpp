#include "RenderCore/RenderAPI.h"

SPtr<Device> RenderAPI::device;

Device *RenderAPI::GetDevice()
{
    return device.get();
}

Device *RenderAPI::CreateDevice(RenderWindow *window, const DeviceDesc &desc)
{
    CT_CHECK(!device);
    device = Device::Create(window, desc);
    return device.get();
}

//====================================================================

namespace
{

struct InternalCache
{
    SPtr<Sampler> defaultSampler;
    SPtr<Texture> defaultTexture2D;
    SPtr<VertexArray> quadVao;
};

InternalCache cache;

}

SPtr<Sampler> RenderAPI::GetDefaultSampler()
{
    if (!cache.defaultSampler)
    {
        cache.defaultSampler = Sampler::Create({});
    }
    return cache.defaultSampler;
}

SPtr<Texture> RenderAPI::GetDefaultTexture2D()
{
    if (!cache.defaultTexture2D)
    {
        uchar8 data[] = { 255, 255, 255, 255 };
        cache.defaultTexture2D = Texture::Create2D(1, 1, ResourceFormat::RGBA8Unorm, 1, 1, data);
    }
    return cache.defaultTexture2D;
}

SPtr<VertexArray> RenderAPI::GetDefaultQuadVertexArray()
{
    if (!cache.quadVao)
    {
        float vertices[] = {
            -1.0f, 1.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 1.0f
        };

        auto vbo = Buffer::Create(sizeof(vertices), ResourceBind::Vertex, BufferCpuAccess::Write, vertices);
        auto layout = VertexBufferLayout::Create(
            { { 0, CT_TEXT("inPos"), ResourceFormat::RG32Float },
              { 1, CT_TEXT("inUV"), ResourceFormat::RG32Float } });
        auto vertexLayout = VertexLayout::Create();
        vertexLayout->AddBufferLayout(layout);
        auto vao = VertexArray::Create();
        vao->SetTopology(Topology::TriangleStrip);
        vao->SetVertexLayout(vertexLayout);
        vao->AddVertexBuffer(vbo);

        cache.quadVao = vao;
    }
    return cache.quadVao;
}