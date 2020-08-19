#include "RenderCore/.Package.h"

static ResourceFormatDesc formatDescs[] = 
{
    //                                                    bytes        componentCount                            isDepth isStencil isCompressed  compressionRatio                                            componentBits
    {ResourceFormat::Unknown,            "Unknown",         0,              0,  ResourceComponentType::Unknown,    {false,  false, false,},        {1, 1},                                                  {0, 0, 0, 0    }},
    {ResourceFormat::R8Unorm,            "R8Unorm",         1,              1,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {8, 0, 0, 0    }},
    {ResourceFormat::R8Snorm,            "R8Snorm",         1,              1,  ResourceComponentType::Snorm,      {false,  false, false,},        {1, 1},                                                  {8, 0, 0, 0    }},
    {ResourceFormat::R16Unorm,           "R16Unorm",        2,              1,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {16, 0, 0, 0   }},
    {ResourceFormat::R16Snorm,           "R16Snorm",        2,              1,  ResourceComponentType::Snorm,      {false,  false, false,},        {1, 1},                                                  {16, 0, 0, 0   }},
    {ResourceFormat::RG8Unorm,           "RG8Unorm",        2,              2,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {8, 8, 0, 0    }},
    {ResourceFormat::RG8Snorm,           "RG8Snorm",        2,              2,  ResourceComponentType::Snorm,      {false,  false, false,},        {1, 1},                                                  {8, 8, 0, 0    }},
    {ResourceFormat::RG16Unorm,          "RG16Unorm",       4,              2,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {16, 16, 0, 0  }},
    {ResourceFormat::RG16Snorm,          "RG16Snorm",       4,              2,  ResourceComponentType::Snorm,      {false,  false, false,},        {1, 1},                                                  {16, 16, 0, 0  }},
    {ResourceFormat::RGB16Unorm,         "RGB16Unorm",      6,              3,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {16, 16, 16, 0 }},
    {ResourceFormat::RGB16Snorm,         "RGB16Snorm",      6,              3,  ResourceComponentType::Snorm,      {false,  false, false,},        {1, 1},                                                  {16, 16, 16, 0 }},
    {ResourceFormat::R24UnormX8,         "R24UnormX8",      4,              2,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {24, 8, 0, 0   }},
    {ResourceFormat::RGB5A1Unorm,        "RGB5A1Unorm",     2,              4,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {5, 5, 5, 1    }},
    {ResourceFormat::RGBA8Unorm,         "RGBA8Unorm",      4,              4,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {8, 8, 8, 8    }},
    {ResourceFormat::RGBA8Snorm,         "RGBA8Snorm",      4,              4,  ResourceComponentType::Snorm,      {false,  false, false,},        {1, 1},                                                  {8, 8, 8, 8    }},
    {ResourceFormat::RGB10A2Unorm,       "RGB10A2Unorm",    4,              4,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {10, 10, 10, 2 }},
    {ResourceFormat::RGB10A2UInt,        "RGB10A2UInt",     4,              4,  ResourceComponentType::UInt,       {false,  false, false,},        {1, 1},                                                  {10, 10, 10, 2 }},
    {ResourceFormat::RGBA16Unorm,        "RGBA16Unorm",     8,              4,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {16, 16, 16, 16}},
    {ResourceFormat::RGBA8UnormSrgb,     "RGBA8UnormSrgb",  4,              4,  ResourceComponentType::UnormSrgb,  {false,  false, false,},        {1, 1},                                                  {8, 8, 8, 8    }},
    {ResourceFormat::R16Float,           "R16Float",        2,              1,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {16, 0, 0, 0   }},
    {ResourceFormat::RG16Float,          "RG16Float",       4,              2,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {16, 16, 0, 0  }},
    {ResourceFormat::RGB16Float,         "RGB16Float",      6,              3,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {16, 16, 16, 0 }},
    {ResourceFormat::RGBA16Float,        "RGBA16Float",     8,              4,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {16, 16, 16, 16}},
    {ResourceFormat::R32Float,           "R32Float",        4,              1,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {32, 0, 0, 0   }},
    {ResourceFormat::R32FloatX32,        "R32FloatX32",     8,              2,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {32, 32, 0, 0  }},
    {ResourceFormat::RG32Float,          "RG32Float",       8,              2,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {32, 32, 0, 0  }},
    {ResourceFormat::RGB32Float,         "RGB32Float",      12,             3,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {32, 32, 32, 0 }},
    {ResourceFormat::RGBA32Float,        "RGBA32Float",     16,             4,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {32, 32, 32, 32}},
    {ResourceFormat::R11G11B10Float,     "R11G11B10Float",  4,              3,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {11, 11, 10, 0 }},
    {ResourceFormat::RGB9E5Float,        "RGB9E5Float",     4,              3,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {9, 9, 9, 5    }},
    {ResourceFormat::R8Int,              "R8Int",           1,              1,  ResourceComponentType::Int,        {false,  false, false,},        {1, 1},                                                  {8, 0, 0, 0    }},
    {ResourceFormat::R8UInt,             "R8UInt",          1,              1,  ResourceComponentType::UInt,       {false,  false, false,},        {1, 1},                                                  {8, 0, 0, 0    }},
    {ResourceFormat::R16Int,             "R16Int",          2,              1,  ResourceComponentType::Int,        {false,  false, false,},        {1, 1},                                                  {16, 0, 0, 0   }},
    {ResourceFormat::R16UInt,            "R16UInt",         2,              1,  ResourceComponentType::UInt,       {false,  false, false,},        {1, 1},                                                  {16, 0, 0, 0   }},
    {ResourceFormat::R32Int,             "R32Int",          4,              1,  ResourceComponentType::Int,        {false,  false, false,},        {1, 1},                                                  {32, 0, 0, 0   }},
    {ResourceFormat::R32UInt,            "R32UInt",         4,              1,  ResourceComponentType::UInt,       {false,  false, false,},        {1, 1},                                                  {32, 0, 0, 0   }},
    {ResourceFormat::RG8Int,             "RG8Int",          2,              2,  ResourceComponentType::Int,        {false,  false, false,},        {1, 1},                                                  {8, 8, 0, 0    }},
    {ResourceFormat::RG8UInt,            "RG8UInt",         2,              2,  ResourceComponentType::UInt,       {false,  false, false,},        {1, 1},                                                  {8, 8, 0, 0    }},
    {ResourceFormat::RG16Int,            "RG16Int",         4,              2,  ResourceComponentType::Int,        {false,  false, false,},        {1, 1},                                                  {16, 16, 0, 0  }},
    {ResourceFormat::RG16UInt,           "RG16UInt",        4,              2,  ResourceComponentType::UInt,       {false,  false, false,},        {1, 1},                                                  {16, 16, 0, 0  }},
    {ResourceFormat::RG32Int,            "RG32Int",         8,              2,  ResourceComponentType::Int,        {false,  false, false,},        {1, 1},                                                  {32, 32, 0, 0  }},
    {ResourceFormat::RG32UInt,           "RG32UInt",        8,              2,  ResourceComponentType::UInt,       {false,  false, false,},        {1, 1},                                                  {32, 32, 0, 0  }},

    {ResourceFormat::RGB16Int,           "RGB16Int",        6,              3,  ResourceComponentType::Int,        {false,  false, false,},        {1, 1},                                                  {16, 16, 16, 0 }},
    {ResourceFormat::RGB16UInt,          "RGB16UInt",       6,              3,  ResourceComponentType::UInt,       {false,  false, false,},        {1, 1},                                                  {16, 16, 16, 0 }},
    {ResourceFormat::RGB32Int,           "RGB32Int",       12,              3,  ResourceComponentType::Int,        {false,  false, false,},        {1, 1},                                                  {32, 32, 32, 0 }},
    {ResourceFormat::RGB32UInt,          "RGB32UInt",      12,              3,  ResourceComponentType::UInt,       {false,  false, false,},        {1, 1},                                                  {32, 32, 32, 0 }},
    {ResourceFormat::RGBA8Int,           "RGBA8Int",        4,              4,  ResourceComponentType::Int,        {false,  false, false,},        {1, 1},                                                  {8, 8, 8, 8    }},
    {ResourceFormat::RGBA8UInt,          "RGBA8UInt",       4,              4,  ResourceComponentType::UInt,       {false, false, false, },        {1, 1},                                                  {8, 8, 8, 8    }},
    {ResourceFormat::RGBA16Int,          "RGBA16Int",       8,              4,  ResourceComponentType::Int,        {false,  false, false,},        {1, 1},                                                  {16, 16, 16, 16}},
    {ResourceFormat::RGBA16UInt,         "RGBA16UInt",      8,              4,  ResourceComponentType::UInt,       {false,  false, false,},        {1, 1},                                                  {16, 16, 16, 16}},
    {ResourceFormat::RGBA32Int,          "RGBA32Int",      16,              4,  ResourceComponentType::Int,        {false,  false, false,},        {1, 1},                                                  {32, 32, 32, 32}},
    {ResourceFormat::RGBA32UInt,         "RGBA32UInt",     16,              4,  ResourceComponentType::UInt,       {false,  false, false,},        {1, 1},                                                  {32, 32, 32, 32}},
    {ResourceFormat::BGRA8Unorm,         "BGRA8Unorm",      4,              4,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {8, 8, 8, 8    }},
    {ResourceFormat::BGRA8UnormSrgb,     "BGRA8UnormSrgb",  4,              4,  ResourceComponentType::UnormSrgb,  {false,  false, false,},        {1, 1},                                                  {8, 8, 8, 8    }},
    {ResourceFormat::BGRX8Unorm,         "BGRX8Unorm",      4,              4,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {8, 8, 8, 8    }},
    {ResourceFormat::BGRX8UnormSrgb,     "BGRX8UnormSrgb",  4,              4,  ResourceComponentType::UnormSrgb,  {false,  false, false,},        {1, 1},                                                  {8, 8, 8, 8    }},
    {ResourceFormat::Alpha8Unorm,        "Alpha8Unorm",     1,              1,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {0, 0, 0, 8    }},
    {ResourceFormat::Alpha32Float,       "Alpha32Float",    4,              1,  ResourceComponentType::Float,      {false,  false, false,},        {1, 1},                                                  {0, 0, 0, 32   }},

    {ResourceFormat::R5G6B5Unorm,        "R5G6B5Unorm",     2,              3,  ResourceComponentType::Unorm,      {false,  false, false,},        {1, 1},                                                  {5, 6, 5, 0    }},
    {ResourceFormat::D32Float,           "D32Float",        4,              1,  ResourceComponentType::Float,      {true,   false, false,},        {1, 1},                                                  {32, 0, 0, 0   }},
    {ResourceFormat::D16Unorm,           "D16Unorm",        2,              1,  ResourceComponentType::Unorm,      {true,   false, false,},        {1, 1},                                                  {16, 0, 0, 0   }},
    {ResourceFormat::D32FloatS8X24,      "D32FloatS8X24",   8,              2,  ResourceComponentType::Float,      {true,   true,  false,},        {1, 1},                                                  {32, 8, 24, 0  }},
    {ResourceFormat::D24UnormS8,         "D24UnormS8",      4,              2,  ResourceComponentType::Unorm,      {true,   true,  false,},        {1, 1},                                                  {24, 8, 0, 0   }},
    {ResourceFormat::BC1Unorm,           "BC1Unorm",        8,              3,  ResourceComponentType::Unorm,      {false,  false, true, },        {4, 4},                                                  {64, 0, 0, 0   }},
    {ResourceFormat::BC1UnormSrgb,       "BC1UnormSrgb",    8,              3,  ResourceComponentType::UnormSrgb,  {false,  false, true, },        {4, 4},                                                  {64, 0, 0, 0   }},
    {ResourceFormat::BC2Unorm,           "BC2Unorm",        16,             4,  ResourceComponentType::Unorm,      {false,  false, true, },        {4, 4},                                                  {128, 0, 0, 0  }},
    {ResourceFormat::BC2UnormSrgb,       "BC2UnormSrgb",    16,             4,  ResourceComponentType::UnormSrgb,  {false,  false, true, },        {4, 4},                                                  {128, 0, 0, 0  }},
    {ResourceFormat::BC3Unorm,           "BC3Unorm",        16,             4,  ResourceComponentType::Unorm,      {false,  false, true, },        {4, 4},                                                  {128, 0, 0, 0  }},
    {ResourceFormat::BC3UnormSrgb,       "BC3UnormSrgb",    16,             4,  ResourceComponentType::UnormSrgb,  {false,  false, true, },        {4, 4},                                                  {128, 0, 0, 0  }},
    {ResourceFormat::BC4Unorm,           "BC4Unorm",        8,              1,  ResourceComponentType::Unorm,      {false,  false, true, },        {4, 4},                                                  {64, 0, 0, 0   }},
    {ResourceFormat::BC4Snorm,           "BC4Snorm",        8,              1,  ResourceComponentType::Snorm,      {false,  false, true, },        {4, 4},                                                  {64, 0, 0, 0   }},
    {ResourceFormat::BC5Unorm,           "BC5Unorm",        16,             2,  ResourceComponentType::Unorm,      {false,  false, true, },        {4, 4},                                                  {128, 0, 0, 0  }},
    {ResourceFormat::BC5Snorm,           "BC5Snorm",        16,             2,  ResourceComponentType::Snorm,      {false,  false, true, },        {4, 4},                                                  {128, 0, 0, 0  }},

    {ResourceFormat::BC6HS16,            "BC6HS16",         16,             3,  ResourceComponentType::Float,      {false,  false, true, },        {4, 4},                                                  {128, 0, 0, 0  }},
    {ResourceFormat::BC6HU16,            "BC6HU16",         16,             3,  ResourceComponentType::Float,      {false,  false, true, },        {4, 4},                                                  {128, 0, 0, 0  }},
    {ResourceFormat::BC7Unorm,           "BC7Unorm",        16,             4,  ResourceComponentType::Unorm,      {false,  false, true, },        {4, 4},                                                  {128, 0, 0, 0  }},
    {ResourceFormat::BC7UnormSrgb,       "BC7UnormSrgb",    16,             4,  ResourceComponentType::UnormSrgb,  {false,  false, true, },        {4, 4},                                                  {128, 0, 0, 0  }},
};

const ResourceFormatDesc &GetResourceFormatDesc(ResourceFormat format)
{
    return formatDescs[static_cast<int32>(format)];
}

ResourceComponentType GetResourceComponentType(ResourceFormat format)
{
    return GetResourceFormatDesc(format).componentType;
}

uint32 GetResourceFormatBytes(ResourceFormat format)
{
    return GetResourceFormatDesc(format).bytes;
}

uint32 GetResourceFormatComponentCount(ResourceFormat format)
{
    return GetResourceFormatDesc(format).componentCount;
}

uint32 GetResourceFormatWidthCompressionRatio(ResourceFormat format)
{
    return GetResourceFormatDesc(format).compressionRatio.width;
}

uint32 GetResourceFormatHeightCompressionRatio(ResourceFormat format)
{
    return GetResourceFormatDesc(format).compressionRatio.height;
}

bool IsCompressedFormat(ResourceFormat format)
{
    return GetResourceFormatDesc(format).isCompressed;
}

bool IsDepthFormat(ResourceFormat format)
{
    return GetResourceFormatDesc(format).isDepth;
}

bool IsStencilFormat(ResourceFormat format)
{
    return GetResourceFormatDesc(format).isStencil;
}

bool IsDepthStencilFormat(ResourceFormat format)
{
    const auto &desc = GetResourceFormatDesc(format);
    return desc.isDepth || desc.isStencil;
}

bool IsSrgbFormat(ResourceFormat format)
{
    return GetResourceFormatDesc(format).componentType == ResourceComponentType::UnormSrgb;
}

ResourceFormat LinearToSrgbFormat(ResourceFormat format)
{
    switch (format)
    {
    case ResourceFormat::BC1Unorm:
        return ResourceFormat::BC1UnormSrgb;
    case ResourceFormat::BC2Unorm:
        return ResourceFormat::BC2UnormSrgb;
    case ResourceFormat::BC3Unorm:
        return ResourceFormat::BC3UnormSrgb;
    case ResourceFormat::BGRA8Unorm:
        return ResourceFormat::BGRA8UnormSrgb;
    case ResourceFormat::BGRX8Unorm:
        return ResourceFormat::BGRX8UnormSrgb;
    case ResourceFormat::RGBA8Unorm:
        return ResourceFormat::RGBA8UnormSrgb;
    case ResourceFormat::BC7Unorm:
        return ResourceFormat::BC7UnormSrgb;
    default:
        return format;
    }
}

ResourceFormat SrgbToLinearFormat(ResourceFormat format)
{
    switch (format)
    {
    case ResourceFormat::BC1UnormSrgb:
        return ResourceFormat::BC1Unorm;
    case ResourceFormat::BC2UnormSrgb:
        return ResourceFormat::BC2Unorm;
    case ResourceFormat::BC3UnormSrgb:
        return ResourceFormat::BC3Unorm;
    case ResourceFormat::BGRA8UnormSrgb:
        return ResourceFormat::BGRA8Unorm;
    case ResourceFormat::BGRX8UnormSrgb:
        return ResourceFormat::BGRX8Unorm;
    case ResourceFormat::RGBA8UnormSrgb:
        return ResourceFormat::RGBA8Unorm;
    case ResourceFormat::BC7UnormSrgb:
        return ResourceFormat::BC7Unorm;
    default:
        CT_CHECK(!IsSrgbFormat(format));
        return format;
    }
}