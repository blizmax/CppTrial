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

Vector3 ToVector3(const aiVector3D &aVec)
{
    return Vector3(aVec.x, aVec.y, aVec.z);
}

Vector2 ToVector2(const aiVector3D &aVec)
{
    return Vector2(aVec.x, aVec.y);
}

Matrix4 ToMatrix4(const aiMatrix4x4 &aMat)
{
    //TODO
    Matrix4 m;
    return m;
}

class ImporterImpl
{
public:
    using Node = SceneBuilder::Node;
    using Mesh = SceneBuilder::Mesh;

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
            CT_LOG(Error, CT_TEXT("Load scene failed, error: {0}."), String(aImporter.GetErrorString()));
            return nullptr;
        }

        if (CreateMaterials() == false)
        {
            CT_LOG(Error, CT_TEXT("Create scene materials failed."));
            return nullptr;
        }

        if (CreateSceneGraph() == false)
        {
            CT_LOG(Error, CT_TEXT("Create scene graph failed."));
            return nullptr;
        }

        if (CreateMeshes() == false)
        {
            CT_LOG(Error, CT_TEXT("Create scene meshes failed."), path);
            return nullptr;
        }

        if (CreateCamera() == false)
        {
            CT_LOG(Error, CT_TEXT("Create scene camera failed."), path);
            return nullptr;
        }

        if (CreateLights() == false)
        {
            CT_LOG(Error, CT_TEXT("Create scene lights failed."), path);
            return nullptr;
        }

        //TODO Add meshes to model

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

        for (const auto &e : textureMappings)
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

    bool CreateMaterial(const aiMaterial *aMat)
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

        materials.Add(mat);
        return true;
    }

    bool CreateMaterials()
    {
        for (uint32 i = 0; i < aScene->mNumMaterials; ++i)
        {
            if (!CreateMaterial(aScene->mMaterials[i]))
                return false;
        }
        return true;
    }

    int32 GetNodeIndex(const aiNode *aNode)
    {
        if (!aNode)
            return -1;
        auto ptr = nodeIndexMap.TryGet(aNode);
        if (!ptr)
            return -1;
        return *ptr;
    }

    bool ParseNode(const aiNode *aNode)
    {
        int32 newID = nodes.Count();
        Node node = {};

        node.name = String(aNode->mName.C_Str());
        node.transform = ToMatrix4(aNode->mTransformation);
        node.parent = GetNodeIndex(aNode->mParent);
        nodes.Add(std::move(node));
        nodeIndexMap.Put(aNode, newID);

        bool ret = true;
        for (uint32 i = 0; i < aNode->mNumChildren; ++i)
            ret |= ParseNode(pCurrent->mChildren[i]);
        return ret;
    }

    bool CreateSceneGraph()
    {
        auto root = aScene->mRootNode;
        return ParseNode(root);
    }

    bool CreateMesh(const aiMesh *aMesh)
    {
        int32 newID = meshes.Count();
        Mesh mesh = {};

        const uint32 perFaceIndexCount = aMesh->mFaces[0].mNumIndices;
        const uint32 indexCount = aMesh->mNumFaces * perFaceIndexCount;
        mesh.indices.Reserve(indexCount);
        for (uint32 i = 0; i < aMesh->mNumFaces; ++i)
        {
            CT_CHECK(aMesh->mFaces[i].mNumIndices == perFaceIndexCount); //If mesh contains more than one primitive, use aiProcess_SortByPType
            for (uint32 j = 0; j < perFaceIndexCount; ++j)
            {
                mesh.indices.Add(aMesh->mFaces[i].mIndices[j]);
            }
        }

        const uint32 vertexCount = aMesh->mNumVertices;
        mesh.positions.Reserve(vertexCount);
        mesh.normals.Reserve(vertexCount);
        mesh.bitangents.Reserve(vertexCount);
        mesh.uvs.Reserve(vertexCount);
        for (uint32 i = 0; i < aMesh->mNumVertices; ++i)
        {
            mesh.positions.Add(ToVector3(aMesh->mVertices[i]));
            mesh.normals.Add(ToVector3(aMesh->mNormals[i]));
            mesh.bitangents.Add(ToVector3(aMesh->mBitangents[i]));

            if (aMesh->HasTextureCoords(0))
                mesh.uvs.Add(ToVector2(aMesh->mTextureCoords[0][i]));
            else
                mesh.uvs.Add(Vector2());
        }

        switch (aMesh->mFaces[0].mNumIndices)
        {
        case 1:
            mesh.topology = Topology::PointList;
            break;
        case 2:
            mesh.topology = Topology::LineList;
            break;
        case 3:
            mesh.topology = Topology::TriangleList;
            break;
        }
        CT_CHECK(mesh.topology != Topology::Undefined);

        mesh.material = materials[aMesh->mMaterialIndex];

        meshes.Add(std::move(mesh));
        return true;
    }

    bool CreateMeshes()
    {
        for (uint32 i = 0; i < aScene->mNumMeshes; ++i)
        {
            if (!CreateMesh(aScene->mMeshes[i]))
                return false;
        }
        return true;
    }

    bool CreateCamera()
    {
        if (aScene->mNumCameras == 0)
        {
            CT_LOG(Warning, CT_TEXT("Scene has no default camera."));
            return true;
        }
        if (aScene->mNumCameras > 1)
        {
            CT_LOG(Warning, CT_TEXT("Scene has more than one camera, only the first camera will be used."));
        }

        const auto aCamera = aScene->mCameras[0];
        auto camera = Camera::Create();

        //TODO

        return true;
    }

    bool CreateLights()
    {
        //TODO
        return true;
    }

private:
    const aiScene *aScene = nullptr;
    String directory;
    SPtr<SceneImportSettings> settings;
    //SceneBuilder &builder;
    HashMap<String, SPtr<Texture>> textureCache;
    Array<SPtr<Material>> materials;
    Array<Node> nodes;
    Array<Mesh> meshes;
    HashMap<const aiNode*, int32> nodeIndexMap;
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