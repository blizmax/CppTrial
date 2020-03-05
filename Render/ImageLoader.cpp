#include "Render/ImageLoader.h"
#include "IO/FileHandle.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x) CT_ASSERT(x)
#include "stb_image.h"

void *ImageLoader::Load(const String &path, int32 width, int32 height, int32 channels, bool flipY)
{
    IO::FileHandle file(path);
    auto bytes = file.ReadBytes();
    stbi_set_flip_vertically_on_load(flipY ? 1 : 0);
    void *data = stbi_load_from_memory(bytes.GetData(), (int)bytes.Size(), &width, &height, &channels, 0);
    if (!data)
        CT_LOG(Error, "Load image failed. Path: {0}", path);

    return data;
}

void ImageLoader::Free(void *data)
{
    stbi_image_free(data);
}