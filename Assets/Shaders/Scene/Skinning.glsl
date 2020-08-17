#include "Scene/.Package.glsl"

layout(binding = 0) buffer StaticVertexBuffer
{
    StaticVertexData staticVertices[];
};

layout(binding = 1) buffer DynamicVertexBuffer
{
    DynamicVertexData dynamicVertices[];
};

layout(binding = 2) buffer SkinnedVertexBuffer
{
    StaticVertexData skinnedVertices[];
};

layout(binding = 3) buffer PrevSkinnedVertexBuffer
{
    PrevVertexData prevSkinnedVertices[];
};

layout(binding = 4) buffer BoneMatrixBuffer
{
    mat4 boneMatrices[];
};

layout(binding = 5) buffer InverseTransposeBoneMatrixBuffer
{
    mat4 inverseTransposeBoneMatrices[];
};

layout(binding = 6) buffer WorldMatrixBuffer
{
    mat4 worldMatrices[];
};

layout(binding = 7) buffer InverseTransposeWorldMatrixBuffer
{
    mat4 inverseTransposeWorldMatrices[];
};

mat4 GetBoneMatrix(int matrixID)
{
    return boneMatrices[matrixID];
}

mat4 GetInverseTransposeBoneMatrix(int matrixID)
{
    return inverseTransposeBoneMatrices[matrixID];
}

mat4 GetTransposeWorldMatrix(int matrixID)
{
    return transpose(worldMatrices[matrixID]);
}

mat4 GetInverseWorldMatrix(int matrixID)
{
    return transpose(inverseTransposeWorldMatrices[matrixID]);
}

mat4 GetBlendedMatrix(int vertexID)
{
    DynamicVertexData d = dynamicVertices[vertexID];

    mat4 mat = GetBoneMatrix(d.boneID.x) * d.boneWeight.x;
    mat += GetBoneMatrix(d.boneID.y) * d.boneWeight.y;
    mat += GetBoneMatrix(d.boneID.z) * d.boneWeight.z;
    mat += GetBoneMatrix(d.boneID.w) * d.boneWeight.w;
    
    return GetInverseWorldMatrix(d.globalMatrixID) * mat;
}

mat4 GetInverseTransposeBlendedMatrix(int vertexID)
{
    DynamicVertexData d = dynamicVertices[vertexID];

    mat4 mat = GetInverseTransposeBoneMatrix(d.boneID.x) * d.boneWeight.x;
    mat += GetInverseTransposeBoneMatrix(d.boneID.y) * d.boneWeight.y;
    mat += GetInverseTransposeBoneMatrix(d.boneID.z) * d.boneWeight.z;
    mat += GetInverseTransposeBoneMatrix(d.boneID.w) * d.boneWeight.w;

    return GetTransposeWorldMatrix(d.globalMatrixID) * mat;
}

int GetStaticVertexID(int vertexID)
{
    return dynamicVertices[vertexID].staticIndex;
}

StaticVertexData GetStaticVertexData(int vertexID)
{
    return staticVertices[GetStaticVertexID(vertexID)];
}

vec3 GetCurrentPosition(int vertexID)
{
    return skinnedVertices[GetStaticVertexID(vertexID)].position;
}

#type compute

layout(local_size_x = 256, local_size_y = 1) in;

void main()
{
    int vertexID = int(gl_GlobalInvocationID.x);
    int vertexCount = dynamicVertices.length();
    if (vertexID >= vertexCount)
        return;
    
    PrevVertexData prev;
    prev.position = GetCurrentPosition(vertexID);

    StaticVertexData s = GetStaticVertexData(vertexID);
    mat4 boneMat = GetBlendedMatrix(vertexID);
    mat4 invTransposeMat = GetInverseTransposeBlendedMatrix(vertexID);

    s.position = (boneMat * vec4(s.position, 1.0)).xyz;
    s.bitangent = mat3(boneMat) * s.bitangent;
    s.normal = mat3(transpose(invTransposeMat)) * s.normal;

    // store
    skinnedVertices[GetStaticVertexID(vertexID)] = s;
    prevSkinnedVertices[GetStaticVertexID(vertexID)] = prev;
}