#include "Render/Importers/SceneImporter.h"
#include "Core/HashMap.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace
{
enum class TextureType
{
    BaseColor,
    Specular,
    Emissive,
    Normal,
    Occlusion,
};

struct TextureMapping
{
    aiTextureType aType;
    TextureType textureType;
};

static const Array<TextureMapping> textureMappings = {
    {aiTextureType_DIFFUSE, TextureType::BaseColor},
    {aiTextureType_SPECULAR, TextureType::Specular},
    {aiTextureType_EMISSIVE, TextureType::Emissive},
    {aiTextureType_NORMALS, TextureType::Normal},
    {aiTextureType_AMBIENT, TextureType::Occlusion},
};

class ImporterImpl
{
public:
    SPtr<Scene> Import(const String &path, const SPtr<SceneImportSettings> &settings)
    {
        this->settings = settings;
        directory = path.Substring(0, path.LastIndexOf(CT_TEXT("/")));

        uint32 assimpFlags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs;
        assimpFlags &= ~aiProcess_RemoveRedundantMaterials;
        Assimp::Importer aImporter;
        auto u8str = StringEncode::UTF8::ToChars(path);
        aScene = aImporter.ReadFile(u8str.GetData(), assimpFlags);
        if (aScene == nullptr || aScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
        {
            CT_LOG(Error, CT_TEXT("Load scene failed, path: {0}, error: {1}."), path, String(aImporter.GetErrorString()));
            return nullptr;
        }

        CT_CHECK(CreateMaterials());

        return nullptr;
    }

    void LoadTextures(const SPtr<Material> &mat, const aiMaterial *aMat)
    {
        for(const auto &e : textureMappings)
        {
            if (aMat->GetTextureCount(e.aType) > 0)
            {
                aiString aPath;
                aMat->GetTexture(e.aType, 0, &aPath);
                
                String texPath = String(aPath.data);
                SPtr<Texture> texture;
                auto texPtr = textureCache.TryGet(texPath);
                if (texPtr)
                {
                    texture = *texPtr;
                }
                else
                {
                    String fullPath = directory + CT_TEXT("/") + texPath;
                    //texture = 
                }

                CT_CHECK(texture);

                //TODO Set Texture
                switch (e.textureType)
                {
                case TextureType::BaseColor:
                    break;
                case TextureType::Specular:
                    break;
                case TextureType::Emissive:
                    break;
                case TextureType::Normal:
                    break;
                case TextureType::Occlusion:
                    break;
                }
            }
        }
    }

    SPtr<Material> CreateMaterial(const aiMaterial *aMat)
    {
        aiString aName;
        aMat->Get(AI_MATKEY_NAME, aName);

        auto mat = Material::Create(aName.C_Str());
        //TODO

        return mat;
    }

    bool CreateMaterials()
    {
        for (uint32 i = 0; i < aScene->mNumMaterials; ++i)
        {
            auto mat = CreateMaterial(aScene->mMaterials[i]);
            if (mat != nullptr)
                materialMap.Put(i, mat);
            else
                return false;
        }
        return true;
    }

private:
    const aiScene *aScene = nullptr;
    String directory;
    SPtr<SceneImportSettings> settings;
    //SceneBuilder &builder;
    HashMap<uint32, SPtr<Material>> materialMap;
    HashMap<String, SPtr<Texture>> textureCache;
};
}

APtr<Scene> SceneImporter::Import(const String &path, const SPtr<ImportSettings> &settings)
{
    APtr<Scene> result;

    ImporterImpl impl;
    auto scene = impl.Import(path, ImportSettings::As<SceneImportSettings>(settings));

    result.SetData(APtr<Scene>::InnerData::Create(scene));
    return result;
}