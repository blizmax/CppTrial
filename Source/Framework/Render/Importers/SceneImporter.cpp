#include "Render/Importers/SceneImporter.h"
#include "Core/HashMap.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ImporterImpl
{
public:
    enum class TextureType
    {
        Base,
        Specular,
        Emissive,
        Normal,
        Occlusion,
    };

    ImporterImpl(const aiScene *scene) : scene(scene)
    {
    }

    void LoadTextures(const aiMaterial *aiMat)
    {
        //TODO
    }

    SPtr<Material> CreateMaterial(const aiMaterial *aiMat)
    {
        aiString name;
        aiMat->Get(AI_MATKEY_NAME, name);

        auto mat = Material::Create(name.C_Str());
        //TODO

        return mat;
    }

    bool CreateMaterials()
    {
        for (uint32 i = 0; i < scene->mNumMaterials; ++i)
        {
            auto mat = CreateMaterial(scene->mMaterials[i]);
            if (mat != nullptr)
            {
                materialMap.Put(i, mat);
            }
            else
            {
                return false;
            }
        }
        return true;
    }

private:
    const aiScene *scene;
    //SceneBuilder &builder;
    HashMap<uint32, SPtr<Material>> materialMap;
    //HashMap<>
};

APtr<Scene> SceneImporter::Import(const String &path, const SPtr<ImportSettings> &settings)
{
    uint32 assimpFlags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs;
    Assimp::Importer importer;
    auto u8str = StringEncode::UTF8::ToChars(path);
    const auto scene = importer.ReadFile(u8str.GetData(), assimpFlags);

    if (scene == nullptr)
    {
        CT_LOG(Error, CT_TEXT("Load scene failed, path: {0}."), path);
        return nullptr;
    }

    ImporterImpl impl(scene);
    CT_CHECK(impl.CreateMaterials());

    return nullptr;

}