#include "Application/Application.h"
#include "IO/FileHandle.h"
#include "Math/Matrix4.h"
#include "Math/Quat.h"
#include "Utils/DebugTimer.h"

#undef DEBUG
#include <assimp/DefaultLogger.hpp>
#include <assimp/Importer.hpp>
#include <assimp/pbrmaterial.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace
{
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
    return Matrix4(
        aMat.a1, aMat.a2, aMat.a3, aMat.a4,
        aMat.b1, aMat.b2, aMat.b3, aMat.b4,
        aMat.c1, aMat.c2, aMat.c3, aMat.c4,
        aMat.d1, aMat.d2, aMat.d3, aMat.d4);
}

struct Vertex
{
    Color diffuse;
    Vector3 tangent;
    Vector3 bitangent;
    Vector2 uv;
    Vector2 uv2;
};

struct Mesh
{
    //TODO texture

    Color diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };
    Color specular{ 1.0f, 1.0f, 1.0f, 1.0f };
    Color ambient{ 0.0f, 0.0f, 0.0f, 1.0f };
    Color emissive{ 0.0f, 0.0f, 0.0f, 1.0f };

    float opacity = 1.0f;
    float shininess = 15.0f;
    float specularStrength;
    bool doubleSided;

    String ToString() const
    {
        return String::Format(CT_TEXT("Diffuse:{0}\nSpecular:{1}\nAmbient:{2}\nEmissive:{3}"), diffuse, specular, ambient, emissive);
    }
};


}


class LearnAssimp : public Logic
{
private:
    void DumpLight(int32 index, const aiLight *aLight)
    {
        auto lightType = aLight->mType;
        String name = ToString(aLight->mName);

        CT_LOG(Debug, CT_TEXT("Dump light{0}:{1}"), index, name);
    }

    void DumpAnimation(int32 index, const aiAnimation *aAnim)
    {
        String name = ToString(aAnim->mName);
        double durationInTicks = aAnim->mDuration;
        double ticksPerSecond = aAnim->mTicksPerSecond ? aAnim->mTicksPerSecond : 25;
        double durationInSeconds = durationInTicks / ticksPerSecond;

        CT_LOG(Debug, CT_TEXT("Dump animation{0}:{1}"), index, name);

        for (uint32 i = 0; i < aAnim->mNumChannels; ++i)
        {
            auto aNode = aAnim->mChannels[i];
            String nodeName = ToString(aNode->mNodeName);
            auto posKeys = aNode->mNumPositionKeys;
            auto rotKeys = aNode->mNumRotationKeys;
            auto sclKeys = aNode->mNumScalingKeys;

            CT_LOG(Info, CT_TEXT("Channel{0}:{1}"), i, nodeName);
            CT_LOG(Info, CT_TEXT("<positionKeys>:{0}"), posKeys);
            CT_LOG(Info, CT_TEXT("<rotationKeys>:{0}"), rotKeys);
            CT_LOG(Info, CT_TEXT("<scalingKeys>:{0}"), sclKeys);
        }
    }

    void DumpEmbeddedTexture(int32 index, const aiTexture *aTex)
    {
        String fileName = ToString(aTex->mFilename);
        CT_LOG(Debug, CT_TEXT("Dump embedded texture{0}:{1}"), index, fileName);

        String format = String(aTex->achFormatHint);
        CT_LOG(Info, CT_TEXT("<formatHint> {0}"), format);
        CT_LOG(Info, CT_TEXT("<dimension> width:{0}, height:{1}"), aTex->mWidth, aTex->mHeight);
    }

    void DumpMaterial(int32 index, const aiMaterial *aMat)
    {
        Mesh m;

        aiString aName;
        aMat->Get(AI_MATKEY_NAME, aName);
        String name = ToString(aName);

        CT_LOG(Debug, CT_TEXT("Dump material{0}:{1}"), index, name);

        aiColor3D aColor;
        aiString aPath;
        String fieldName;


#define GET_COLOR(key, field)                                                                        \
    fieldName = #field;                                                                              \
    if (aMat->Get(key, aColor) == AI_SUCCESS)                                                        \
    {                                                                                                \
        m.field = Color(aColor.r, aColor.g, aColor.b, 1.0);                                          \
        CT_LOG(Info, CT_TEXT("<{0}> color value: {1}"), fieldName, m.field);                         \
    }                                                                                                \
    else                                                                                             \
    {                                                                                                \
        CT_LOG(Info, CT_TEXT("<{0}> color is not defined, default value: {1}"), fieldName, m.field); \
    }

#define GET_SCALAR(key, field, type)                                                                 \
    type field;                                                                                      \
    fieldName = #field;                                                                              \
    if (aMat->Get(key, field) == AI_SUCCESS)                                                         \
    {                                                                                                \
        m.field = field;                                                                             \
        CT_LOG(Info, CT_TEXT("<{0}> color value: {1}"), fieldName, m.field);                         \
    }                                                                                                \
    else                                                                                             \
    {                                                                                                \
        CT_LOG(Info, CT_TEXT("<{0}> color is not defined, default value: {1}"), fieldName, m.field); \
    }

#define GET_TEXTURE(key, field)                                     \
    fieldName = #field;                                             \
    String field;                                                   \
    if (aMat->GetTexture(key, 0, &aPath) == AI_SUCCESS)             \
    {                                                               \
        field = ToString(aPath);                                    \
        CT_LOG(Info, CT_TEXT("<{0}> path: {1}"), fieldName, field); \
    }                                                               \
    else                                                            \
    {                                                               \
        CT_LOG(Info, CT_TEXT("<{0}> is not defined"), fieldName);   \
    }

        GET_COLOR(AI_MATKEY_COLOR_DIFFUSE, diffuse)
        GET_COLOR(AI_MATKEY_COLOR_SPECULAR, specular)
        GET_COLOR(AI_MATKEY_COLOR_AMBIENT, ambient)
        GET_COLOR(AI_MATKEY_COLOR_EMISSIVE, emissive)

        GET_SCALAR(AI_MATKEY_OPACITY, opacity, float)
        GET_SCALAR(AI_MATKEY_SHININESS, shininess, float)
        GET_SCALAR(AI_MATKEY_SHININESS_STRENGTH, specularStrength, float)


        GET_TEXTURE(aiTextureType_DIFFUSE, diffuseTexture)
        GET_TEXTURE(aiTextureType_SPECULAR, specularTexture)
        GET_TEXTURE(aiTextureType_OPACITY, opacityTexture)
        GET_TEXTURE(aiTextureType_AMBIENT, ambientTexture)
        GET_TEXTURE(aiTextureType_EMISSIVE, emissiveTexture)
        GET_TEXTURE(aiTextureType_SHININESS, shinessTexture)
        GET_TEXTURE(aiTextureType_LIGHTMAP, lightmapTexture)
        GET_TEXTURE(aiTextureType_NORMALS, normalTexture)
        GET_TEXTURE(aiTextureType_HEIGHT, heightTexture)
        GET_TEXTURE(aiTextureType_DISPLACEMENT, displacementTexture)
        GET_TEXTURE(aiTextureType_REFLECTION, reflectionTexture)


        //GET_TEXTURE(AI_MATKEY_GLOBAL_BACKGROUND_IMAGE, globalBgTexture)


        int shadingModel;
        if (aMat->Get(AI_MATKEY_SHADING_MODEL, shadingModel) == AI_SUCCESS)
        {
            CT_LOG(Info, CT_TEXT("<shadingModel> value: {0}"), shadingModel);
        }
        else
        {
            CT_LOG(Info, CT_TEXT("<shadingModel> is not defined"));
        }
    }

    void DumpMesh(int32 index, const aiMesh *aMesh)
    {
        
    }

public:
    virtual void Startup() override
    {
        //Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);

        Assimp::Importer aImporter;

        uint32 assimpFlags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs;
        assimpFlags &= ~(aiProcess_FindDegenerates);
        assimpFlags &= ~(aiProcess_OptimizeGraph);
        assimpFlags &= ~aiProcess_RemoveRedundantMaterials;

        String path = CT_TEXT("D:/Assets/Models/Bee.glb");
        //String path = CT_TEXT("D:/Assets/Media/Arcade/Arcade.fbx");
        auto aScene = aImporter.ReadFile(CT_U8_CSTR(path), assimpFlags);

        if (aScene == nullptr || aScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
        {
            CT_LOG(Error, CT_TEXT("Load scene failed, error: {0}."), String(aImporter.GetErrorString()));
        }

        //Assimp::DefaultLogger::kill();

        CT_LOG(Info, CT_TEXT("Dump assimp begin========================================"));

        for (uint32 i = 0; i < aScene->mNumLights; ++i)
        {
            DumpLight(i, aScene->mLights[i]);
        }

        for (uint32 i = 0; i < aScene->mNumAnimations; ++i)
        {
            DumpAnimation(i, aScene->mAnimations[i]);
        }

        for (uint32 i = 0; i < aScene->mNumTextures; ++i)
        {
            DumpEmbeddedTexture(i, aScene->mTextures[i]);
        }

        for (uint32 i = 0; i < aScene->mNumMaterials; ++i)
        {
            DumpMaterial(i, aScene->mMaterials[i]);
        }

        for (uint32 i = 0; i < aScene->mNumMeshes; ++i)
        {
            DumpMesh(i, aScene->mMeshes[i]);
        }

        CT_LOG(Info, CT_TEXT("Dump assimp end========================================"));
    }


    virtual void Shutdown() override
    {
    }

    virtual void Tick() override
    {
    }
};

LearnAssimp logic;
Logic *gLogic = &logic;