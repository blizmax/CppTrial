#include "Render/Importers/TextureImporter.h"
#include "Render/ImageLoader.h"

APtr<Texture> TextureImporter::Import(const String &path, const SPtr<ImportSettings> &settings)
{
    APtr<Texture> result;

    int32 w, h, c;
    void *data = ImageLoader::Load(path, w, h, c);
    CT_CHECK(data);
    auto texture = Texture::Create2D(w, h, ResourceFormat::RGBA8Unorm, 1, 1, data);
    ImageLoader::Free(data);

    result.SetData(APtr<Texture>::InnerData::Create(texture));

    return result;
}