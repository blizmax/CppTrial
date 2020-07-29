
// Instance defs
#define CT_MESH_INSTANCE_NONE 0
#define CT_MESH_INSTANCE_FLIPPED 1

// Light defs
#define CT_LIGHT_TYPE_POINT 0
#define CT_LIGHT_TYPE_DIRECTIONAL 1

// Material defs
#define CT_SHADING_MODEL_METAL_ROUGH 0 // Use 8 bits store shading models
#define CT_SHADING_MODEL_SPEC_GLOSS 1
#define CT_SHADING_MODEL_MAX_COUNT 256


#define CT_MAT_SAMPLE_MODE_NONE 0
#define CT_MAT_SAMPLE_MODE_CONST 1
#define CT_MAT_SAMPLE_MODE_TEXTURE 2

#define CT_MAT_NORMAL_MODE_NONE 0
#define CT_MAT_NORMAL_MODE_RGB 1
#define CT_MAT_NORMAL_MODE_RG 2

#define CT_MAT_DIFFUSE 10 // Offset bit
#define CT_MAT_SPECULAR 12
#define CT_MAT_EMISSIVE 14
#define CT_MAT_NORMAL 16
#define CT_MAT_OCCLUSION 18

#define CT_MAT_DOUBLE_SIDED 20

CT_INLINE int32 GetMaterialBits(int32 flags, int32 offset, int32 bits)
{
    return (flags >> offset) & ((1 << bits) - 1);
}

CT_INLINE int32 SetMaterialBits(int32 flags, int32 offset, int32 bits, int32 value)
{
    return ((value & ((1 << bits) - 1)) << offset) | (flags & (~(((1 << bits) - 1) << offset)));
}

CT_INLINE int32 GetMaterialBit(int32 flags, int32 offset)
{
    return GetMaterialBits(flags, offset, 1);
}

CT_INLINE int32 SetMaterialBit(int32 flags, int32 offset, bool value)
{
    return SetMaterialBits(flags, offset, 1, value ? 1 : 0);
}

CT_INLINE int32 GetMaterialSampleMode(int32 flags, int32 mapType)
{
    return GetMaterialBits(flags, mapType, 2);
}

CT_INLINE int32 SetMaterialSampleMode(int32 flags, int32 mapType, int32 mode)
{
    return SetMaterialBits(flags, mapType, 2, mode);
}

CT_INLINE int32 GetMaterialNormalMode(int32 flags)
{
    return GetMaterialBits(flags, CT_MAT_NORMAL, 2);
}

CT_INLINE int32 SetMaterialNormalMode(int32 flags, int32 mode)
{
    return SetMaterialBits(flags, CT_MAT_NORMAL, 2, mode);
}

CT_INLINE int32 GetMaterialShadingModel(int32 flags)
{
    return flags & (CT_SHADING_MODEL_MAX_COUNT - 1);
}

CT_INLINE int32 SetMaterialShadingModel(int32 flags, int32 model)
{
    return (flags & ~(CT_SHADING_MODEL_MAX_COUNT - 1)) | model;
}

struct MeshInstanceData
{
    int32 globalMatrixID;
    int32 materialID;
    int32 meshID;
    int32 flags;
    int32 vertexOffset;
    int32 indexOffset;
};

struct MeshDesc
{
    int32 vertexOffset;
    int32 indexOffset;
    int32 vertexCount;
    int32 indexCount;
    int32 materialID;
};

struct StaticVertexData
{
    Vector3 position;
    Vector3 normal;
    Vector3 bitangent;
    Vector2 uv;
};

struct DynamicVertexData
{
    Vector4I boneID;
    Vector4 boneWeight;
    int32 staticIndex;
    int32 globalMatrixID;
};

struct PrevVertexData
{
    Vector3 position;
};

struct MaterialData
{
    Color base;     //RGB: Base color,     A: Transparency
    Color specular; //RGB: Specular,       A: Gloss       | R: Occluse, G:Roughness, B:Metallic
    Color emissive; //RGB: Emissive color, A: Factor

    float alphaThreshold;
    float IoR;
    float specularTransmission;
    int32 flags;

#ifdef __cplusplus
    MaterialData();
#endif
};

struct LightData
{
    Vector3 posW; // world space position
    int32 type;
    Vector3 dirW; // world space direction
    float openingAngle;
    Vector3 intensity;
    float cosOpeningAngle;
    float penumbraAngle;
    float _pad[3];

#ifdef __cplusplus
    LightData();
#endif
};

struct CameraData
{
    Matrix4 viewMat;
    Matrix4 projMat;
    Matrix4 viewProjMat;
    Matrix4 invViewProj;

    Vector3 posW; // world space position
    float focalLength;
    Vector3 up;
    float aspectRatio; // default is 1.7777(16:9)
    Vector3 target;
    float nearZ;
    Vector3 cameraU;
    float farZ;
    Vector3 cameraV;
    float jitterX;
    Vector3 cameraW;
    float jitterY;

    float frameHeight;
    float focalDistance;
    float apertureRadius;
    float shutterSpeed;

#ifdef __cplusplus
    CameraData();
#endif
};
