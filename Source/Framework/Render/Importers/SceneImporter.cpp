#include "Render/Importers/SceneImporter.h"
#include "Render/Importers/TextureImporter.h"
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

        if (CreateMaterials() == false)
        {
            CT_LOG(Error, CT_TEXT("Create scene materials failed, path: {0}"), path);
            return nullptr;
        }

        if (CreateSceneGraph() == false)
        {
            CT_LOG(Error, CT_TEXT("Create scene graph failed, path: {0}"), path);
            return nullptr;
        }

        return nullptr;
    }

    bool IsSrgbRequired(TextureType textureType)
    {
        switch (textureType)
        {
        case TextureType::BaseColor:
        case TextureType::Emissive:
        case TextureType::Occlusion:
            return true;
        default:
            return false;
        }
    }

    void SetTexture(const SPtr<Material> &mat, const SPtr<Texture> &texture, TextureType textureType)
    {
        switch (textureType)
        {
        case TextureType::BaseColor:
            mat->SetBaseTexture(texture);
            break;
        case TextureType::Specular:
            mat->SetSpecularTexture(texture);
            break;
        case TextureType::Emissive:
            mat->SetEmissiveTexture(texture);
            break;
        case TextureType::Normal:
            mat->SetNormalTexture(texture);
            break;
        case TextureType::Occlusion:
            mat->SetOcclusionTexture(texture);
            break;
        }
    }

    void LoadTextures(const SPtr<Material> &mat, const aiMaterial *aMat)
    {
        bool useSrgb = !settings->assumeLinearSpaceTextures;

        for(const auto &e : textureMappings)
        {
            if (aMat->GetTextureCount(e.aType) > 0)
            {
                aiString aPath;
                aMat->GetTexture(e.aType, 0, &aPath);
                
                String path = String(aPath.data);
                if (path.IsEmpty())
                {
                    CT_LOG(Warning, CT_TEXT("Load scene texture with empty file name, ignored."));
                    continue;
                }

                SPtr<Texture> texture;
                auto texPtr = textureCache.TryGet(path);
                if (texPtr)
                {
                    texture = *texPtr;
                }
                else
                {
                    String fullPath = directory + CT_TEXT("/") + path;
                    auto textureSettings = TextureImportSettings::Create();
                    textureSettings->generateMips = true;
                    textureSettings->srgbFormat = useSrgb && IsSrgbRequired(e.textureType);
                    TextureImporter importer;
                    texture = importer.Import(fullPath, textureSettings);
                    if (texture)
                        textureCache.Put(path, texture);
                }

                CT_CHECK(texture);
                SetTexture(mat, texture, e.textureType);
            }
        }

        //FIXME Should flush?
    }

    SPtr<Material> CreateMaterial(const aiMaterial *aMat)
    {
        aiString aName;
        aMat->Get(AI_MATKEY_NAME, aName);

        auto mat = Material::Create(aName.C_Str());
        
        LoadTextures(mat, aMat);

        float opacity;
        if (aMat->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS)
        {
            auto baseColor = mat->GetBaseColor();
            baseColor.a = opacity;
            mat->SetBaseColor(baseColor);
        }

        float bumpScaling;
        if (aMat->Get(AI_MATKEY_BUMPSCALING, bumpScaling) == AI_SUCCESS)
        {
            //TODO
        }

        float shininess;
        if (aMat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
        {
            auto specular = mat->GetSpecularColor();
            specular.a = shininess;
            mat->SetSpecularColor(specular);
        }

        float refraction;
        if (aMat->Get(AI_MATKEY_REFRACTI, refraction) == AI_SUCCESS)
        {
            mat->SetIndexOfRefraction(refraction);
        }

        aiColor3D aColor;
        if (aMat->Get(AI_MATKEY_COLOR_DIFFUSE, aColor) == AI_SUCCESS)
        {
            Color color(aColor.r, aColor.g, aColor.b, mat->GetBaseColor().a);
            mat->SetBaseColor(color);
        }

        if (aMat->Get(AI_MATKEY_COLOR_SPECULAR, aColor) == AI_SUCCESS)
        {
            Color color(aColor.r, aColor.g, aColor.b, mat->GetSpecularColor().a);
            mat->SetSpecularColor(color);
        }

        if (aMat->Get(AI_MATKEY_COLOR_EMISSIVE, aColor) == AI_SUCCESS)
        {
            Color color(aColor.r, aColor.g, aColor.b, mat->GetEmissiveColor().a);
            mat->SetEmissiveColor(color);
        }

        int doubleSided;
        if (aMat->Get(AI_MATKEY_TWOSIDED, doubleSided) == AI_SUCCESS)
        {
            //TODO
        }

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

    bool CreateSceneGraph()
    {
        //TODO

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