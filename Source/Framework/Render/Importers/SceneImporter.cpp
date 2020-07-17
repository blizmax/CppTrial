#include "Render/Importers/SceneImporter.h"
#include "Core/HashMap.h"
#include "Render/Importers/TextureImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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
    { aiTextureType_DIFFUSE, TextureType::BaseColor },
    { aiTextureType_SPECULAR, TextureType::Specular },
    { aiTextureType_EMISSIVE, TextureType::Emissive },
    { aiTextureType_NORMALS, TextureType::Normal },
    { aiTextureType_AMBIENT, TextureType::Occlusion },
};

String ToString(const aiString &aStr)
{
    return String(aStr.C_Str());
}

Vector3 ToVector3(const aiVector3D &aVec)
{
    return Vector3(aVec.x, aVec.y, aVec.z);
}

Vector2 ToVector2(const aiVector3D &aVec)
{
    return Vector2(aVec.x, aVec.y);
}

Quat ToQuat(const aiQuaternion &aQuat)
{
    return Quat(aQuat.x, aQuat.y, aQuat.z, aQuat.w);
}

Color ToColor(const aiColor3D &aCol)
{
    return Color(aCol.r, aCol.g, aCol.b);
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
    ImporterImpl() = default;

    SPtr<Scene> Import(const String &path, const SPtr<SceneImportSettings> &settings)
    {
        this->settings = settings;
        directory = path.Substring(0, path.LastIndexOf(CT_TEXT("/")));

        uint32 assimpFlags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs;
        assimpFlags &= ~(aiProcess_FindDegenerates);
        assimpFlags &= ~(aiProcess_OptimizeGraph);
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

        if (CreateAnimations() == false)
        {
            CT_LOG(Error, CT_TEXT("Create scene animations failed."), path);
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

        return builder.GetScene();
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

    bool IsBone(const String &name)
    {
        return boneMatrixMap.Contains(name);
    }

    bool IsBone(const aiString &name)
    {
        return boneMatrixMap.Contains(ToString(name));
    }

    int32 GetNodeID(const aiNode *aNode)
    {
        if (!aNode)
            return -1;
        auto ptr = nodePtrToID.TryGet(aNode);
        if (!ptr)
            return -1;
        return *ptr;
    }

    int32 GetNodeID(const String &name, int32 index)
    {
        auto ptr = nodeNameToPtrs.TryGet(name);
        if (!ptr)
            return -1;
        auto &nodes = *ptr;
        if (index < 0 || index >= nodes.Count())
            return -1;
        return GetNodeID(nodes[index]);
    }

    int32 GetNodeCount(const String &name)
    {
        auto ptr = nodeNameToPtrs.TryGet(name);
        if (!ptr)
            return 0;
        auto &nodes = *ptr;
        return nodes.Count();
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
        String name = ToString(aName);

        auto mat = Material::Create();
        mat->SetName(name);

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

    void CreateBoneList()
    {
        for (uint32 i = 0; i < aScene->mNumMeshes; ++i)
        {
            const auto aMesh = aScene->mMeshes[i];
            if (!aMesh->HasBones())
                continue;

            for (uint32 b = 0; b < aMesh->mNumBones; ++b)
            {
                String name = ToString(aMesh->mBones[b]->mName);
                boneMatrixMap[name] = ToMatrix4(aMesh->mBones[b]->mOffsetMatrix);
            }
        }
    }

    bool ParseNode(const aiNode *aNode)
    {
        SceneNode node = {};

        String name = ToString(aNode->mName);
        const bool boneNode = IsBone(name);
        CT_CHECK(!boneNode || aNode->mNumMeshes == 0);

        node.name = name;
        node.transform = ToMatrix4(aNode->mTransformation);
        node.parent = GetNodeID(aNode->mParent);
        if (boneNode)
            node.localToBindPose = boneMatrixMap[name];

        int32 nodeID = builder.AddNode(std::move(node));
        nodePtrToID.Put(aNode, nodeID);
        auto nodesPtr = nodeNameToPtrs.TryGet(name);
        if (!nodesPtr)
            nodeNameToPtrs.Put(name, {});
        nodeNameToPtrs[name].Add(aNode);

        bool ret = true;
        for (uint32 i = 0; i < aNode->mNumChildren; ++i)
            ret |= ParseNode(aNode->mChildren[i]);
        return ret;
    }

    bool CreateSceneGraph()
    {
        CreateBoneList();
        auto root = aScene->mRootNode;
        CT_CHECK(!IsBone(root->mName));
        bool ret = ParseNode(root);

        //TODO Dump

        return ret;
    }

    void LoadBones(const aiMesh *aMesh, Mesh &mesh)
    {
        const auto vertexCount = aMesh->mNumVertices;
        mesh.boneIDs.SetCount(vertexCount);
        mesh.boneWeights.SetCount(vertexCount);

        for (uint32 b = 0; b < aMesh->mNumBones; ++b)
        {
            const auto aBone = aMesh->mBones[b];
            String name = ToString(aBone->mName);
            CT_CHECK(GetNodeCount(name) == 1);
            int32 boneID = GetNodeID(name, 0);
            for (uint32 w = 0; w < aBone->mNumWeights; ++w)
            {
                const auto &aWeight = aBone->mWeights[w];
                auto &vertexIDs = mesh.boneIDs[aWeight.mVertexId];
                auto &vertexWeights = mesh.boneWeights[aWeight.mVertexId];

                bool found = false;
                for (uint32 i = 0; i < 4; ++i)
                {
                    if (vertexWeights[i] == 0.0f)
                    {
                        vertexIDs[i] = boneID;
                        vertexWeights[i] = aWeight.mWeight;
                        found = true;
                        break;
                    }
                }
                if (!found)
                    CT_LOG(Error, CT_TEXT("Create scene bones error, one vertex has more than 4 bones."));
            }
        }

        for (auto &e : mesh.boneWeights)
        {
            float sum = e.x + e.y + e.z + e.w;
            e /= sum;
        }
    }

    bool CreateMesh(const aiMesh *aMesh, uint32 index)
    {
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

        const auto vertexCount = aMesh->mNumVertices;
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

        if (aMesh->HasBones())
        {
            LoadBones(aMesh, mesh);
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

        int32 meshID = builder.AddMesh(std::move(mesh));
        meshIndexToID.Put(index, meshID);

        return true;
    }

    void AddMeshes(const aiNode *aNode)
    {
        int32 nodeID = GetNodeID(aNode);
        for (uint32 i = 0; i < aNode->mNumMeshes; ++i)
        {
            int32 meshID = meshIndexToID[aNode->mMeshes[i]];
            builder.AddMeshInstance(nodeID, meshID);
        }

        for (uint32 i = 0; i < aNode->mNumChildren; ++i)
        {
            AddMeshes(aNode->mChildren[i]);
        }
    }

    bool CreateMeshes()
    {
        for (uint32 i = 0; i < aScene->mNumMeshes; ++i)
        {
            if (!CreateMesh(aScene->mMeshes[i], i))
                return false;
        }

        AddMeshes(aScene->mRootNode);

        return true;
    }

    void ResetKeyframeTimes(aiNodeAnim *aNode)
    {
        auto ResetTime = [](auto keys, uint32 count) {
            if (count > 1)
                CT_CHECK(keys[1].mTime >= 0);

            if (keys[0].mTime < 0)
                keys[0].mTime = 0;
        };

        ResetTime(aNode->mPositionKeys, aNode->mNumPositionKeys);
        ResetTime(aNode->mRotationKeys, aNode->mNumRotationKeys);
        ResetTime(aNode->mScalingKeys, aNode->mNumScalingKeys);
    }

    template <typename AiKeyType, typename ValType, typename Cast>
    bool ParseAnimationKey(const AiKeyType *aKeys, uint32 count, double time, uint32 &currentIndex, ValType &val, const Cast &cast)
    {
        if (currentIndex >= count)
            return true;

        if (aKeys[currentIndex].mTime == time)
        {
            val = cast(aKeys[currentIndex].mValue);
            currentIndex++;
        }
        return currentIndex >= count;
    }

    bool CreateAnimation(const aiAnimation *aAnim)
    {
        String name = ToString(aAnim->mName);
        double durationInTicks = aAnim->mDuration;
        double ticksPerSecond = aAnim->mTicksPerSecond ? aAnim->mTicksPerSecond : 25;
        double durationInSeconds = durationInTicks / ticksPerSecond;

        auto animation = Animation::Create(durationInSeconds);
        animation->SetName(name);

        auto ResetTime = [](auto keys, uint32 count) {
            if (count > 1)
                CT_CHECK(keys[1].mTime >= 0);

            if (keys[0].mTime < 0)
                keys[0].mTime = 0;
        };

        // Parse node animation
        for (uint32 i = 0; i < aAnim->mNumChannels; ++i)
        {
            auto aNode = aAnim->mChannels[i];
            String nodeName = ToString(aNode->mNodeName);
            ResetKeyframeTimes(aNode);

            Array<int32> channels;
            for (int32 n = 0; n < GetNodeCount(nodeName); ++n)
                channels.Add(animation->AddChannel(GetNodeID(nodeName, n)));

            Animation::Keyframe keyframe = {};
            bool done = false;
            uint32 pos = 0, rot = 0, scl = 0;
            auto NextKeyTime = [&]() {
                double t = -1.0; //Assume key time always >= 0
                if (pos < aNode->mNumPositionKeys)
                    t = Math::Max(t, aNode->mPositionKeys[pos].mTime);
                if (rot < aNode->mNumRotationKeys)
                    t = Math::Max(t, aNode->mRotationKeys[rot].mTime);
                if (scl < aNode->mNumScalingKeys)
                    t = Math::Max(t, aNode->mScalingKeys[scl].mTime);
                CT_CHECK(t >= 0.0);
                return t;
            };
            while (!done)
            {
                auto time = NextKeyTime();
                CT_CHECK(time == 0 || (time / ticksPerSecond) > keyframe.time);
                keyframe.time = time / ticksPerSecond;

                done = ParseAnimationKey(aNode->mPositionKeys, aNode->mNumPositionKeys, time, pos, keyframe.translation, ToVector3);
                done = ParseAnimationKey(aNode->mRotationKeys, aNode->mNumRotationKeys, time, rot, keyframe.rotation, ToQuat) && done;
                done = ParseAnimationKey(aNode->mScalingKeys, aNode->mNumScalingKeys, time, scl, keyframe.scaling, ToVector3) && done;

                for (auto c : channels)
                    animation->AddKeyframe(c, keyframe);
            }
        }

        builder.AddAnimation(0, animation);

        return true;
    }

    bool CreateAnimations()
    {
        for (uint32 i = 0; i < aScene->mNumAnimations; ++i)
        {
            if (!CreateAnimation(aScene->mAnimations[i]))
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
        camera = Camera::Create();
        camera->SetPosition(ToVector3(aCamera->mPosition));
        camera->SetUp(ToVector3(aCamera->mUp));
        camera->SetTarget(ToVector3(aCamera->mLookAt) + ToVector3(aCamera->mPosition));

        if (aCamera->mAspect != 0.0f)
            camera->SetAspectRatio(aCamera->mAspect);

        camera->SetNearZ(aCamera->mClipPlaneNear);
        camera->SetFarZ(aCamera->mClipPlaneFar);

        //TODO fov, add node

        builder.SetCamera(camera);

        return true;
    }

    bool AddLightCommon(const SPtr<Light> &light, const aiLight *aLight)
    {
        CT_CHECK(aLight->mColorDiffuse == aLight->mColorSpecular);
        light->SetIntensity(ToColor(aLight->mColorSpecular));

        //TODO add node

        builder.AddLight(light);

        return true;
    }

    bool CreateDirLight(const aiLight *aLight)
    {
        auto light = DirectionalLight::Create();
        light->SetDirection(ToVector3(aLight->mDirection));

        return AddLightCommon(light, aLight);
    }

    bool CreatePointLight(const aiLight *aLight)
    {
        auto light = PointLight::Create();
        light->SetPosition(ToVector3(aLight->mPosition));
        light->SetDirection(ToVector3(aLight->mDirection));
        light->SetOpeningAngle(aLight->mAngleOuterCone);
        light->SetPenumbraAngle(aLight->mAngleOuterCone - aLight->mAngleInnerCone);

        return AddLightCommon(light, aLight);
    }

    bool CreateLights()
    {
        for (uint32 i = 0; i < aScene->mNumLights; ++i)
        {
            const auto aLight = aScene->mLights[i];
            switch (aLight->mType)
            {
            case aiLightSource_DIRECTIONAL:
                if (!CreateDirLight(aLight))
                    return false;
                break;
            case aiLightSource_POINT:
            case aiLightSource_SPOT:
                if (!CreatePointLight(aLight))
                    return false;
                break;
            default:
                CT_LOG(Warning, CT_TEXT("Loading scene includes unsupported light type, just ignore it."));
                break;
            }
        }
        return true;
    }

private:
    SceneBuilder builder;
    SPtr<SceneImportSettings> settings;
    String directory;
    const aiScene *aScene = nullptr;
    HashMap<String, SPtr<Texture>> textureCache;
    Array<SPtr<Material>> materials;
    SPtr<Camera> camera;
    HashMap<const aiNode *, int32> nodePtrToID;
    HashMap<String, Array<const aiNode *>> nodeNameToPtrs;
    HashMap<String, Matrix4> boneMatrixMap;
    HashMap<uint32, int32> meshIndexToID;
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