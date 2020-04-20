#include "RenderCore/.Package.h"

namespace RenderCore
{
uint32 GetVertexDataSize(VertexDataType dataType)
{
    switch (dataType)
    {
    case VertexDataType::Float1:
        return 4;
    case VertexDataType::Float2:
        return 4 * 2;
    case VertexDataType::Float3:
        return 4 * 3;
    case VertexDataType::Float4:
        return 4 * 4;
    case VertexDataType::Int1:
        return 4;
    case VertexDataType::Int2:
        return 4 * 2;
    case VertexDataType::Int3:
        return 4 * 3;
    case VertexDataType::Int4:
        return 4 * 4;
    case VertexDataType::UByte4:
        return 4;
    }
    CT_EXCEPTION(Render, "Unknown data type!");
    return 0;
}

uint32 GetVertexDataComponentCount(VertexDataType dataType)
{
    switch (dataType)
    {
    case VertexDataType::Float1:
        return 1;
    case VertexDataType::Float2:
        return 2;
    case VertexDataType::Float3:
        return 3;
    case VertexDataType::Float4:
        return 4;
    case VertexDataType::Int1:
        return 1;
    case VertexDataType::Int2:
        return 2;
    case VertexDataType::Int3:
        return 3;
    case VertexDataType::Int4:
        return 4;
    case VertexDataType::UByte4:
        return 4;
    }
    CT_EXCEPTION(Render, "Unknown data type!");
    return 0;
}
}