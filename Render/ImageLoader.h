#pragma once

#include "Render/.Package.h"

class ImageLoader
{
public:
    static void *Load(const String &path, int32& width, int32& height, int32& channels, bool flipY = false);
    static void Free(void *data);
};