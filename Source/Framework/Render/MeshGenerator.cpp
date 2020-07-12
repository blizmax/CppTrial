#include "Render/MeshGenerator.h"

void MeshGenerator::Subdivide(MeshData &meshData)
{
    //       v1
    //       *
    //      / \
	//     /   \
	//  m0*-----*m1
    //   / \   / \
	//  /   \ /   \
	// *-----*-----*
    // v0    m2     v2

    MeshData copy = meshData;

    auto &vertices = meshData.vertices;
    auto &indices = meshData.indices;
    vertices.Clear();
    indices.Clear();

    auto MidPoint = [](const auto &v0, const auto &v1) {
        auto v = v0;
        v.position = (v0.position + v1.position) * 0.5f;
        v.normal = (v0.normal + v1.normal).Normalize();
        v.tangent = (v0.tangent + v1.tangent).Normalize();
        v.uv = (v0.uv + v1.uv) * 0.5f;
        return v;
    };

    int32 triangleCount = copy.indices.Count() / 3;
    for (int32 i = 0; i < triangleCount; ++i)
    {
        auto v0 = copy.vertices[copy.indices[i * 3]];
        auto v1 = copy.vertices[copy.indices[i * 3 + 1]];
        auto v2 = copy.vertices[copy.indices[i * 3 + 2]];

        auto m0 = MidPoint(v0, v1);
        auto m1 = MidPoint(v1, v2);
        auto m2 = MidPoint(v0, v2);

        vertices.Add(v0);
        vertices.Add(v1);
        vertices.Add(v2);
        vertices.Add(m0);
        vertices.Add(m1);
        vertices.Add(m2);

        indices.Add(i * 6);
        indices.Add(i * 6 + 3);
        indices.Add(i * 6 + 5);
        indices.Add(i * 6 + 3);
        indices.Add(i * 6 + 4);
        indices.Add(i * 6 + 5);
        indices.Add(i * 6 + 5);
        indices.Add(i * 6 + 4);
        indices.Add(i * 6 + 2);
        indices.Add(i * 6 + 3);
        indices.Add(i * 6 + 1);
        indices.Add(i * 6 + 4);
    }
}

MeshData MeshGenerator::CreateBox(float width, float height, float depth, int32 subdivisions)
{
    MeshData meshData;

    auto &v = meshData.vertices;
    auto &i = meshData.indices;
    v.SetCount(24);
    i.SetCount(36);

    float w2 = 0.5f * width;
    float h2 = 0.5f * height;
    float d2 = 0.5f * depth;

    // Front
    v[0] = {-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    v[1] = {-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    v[2] = {+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    v[3] = {+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f};
    // Back
    v[4] = {-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f};
    v[5] = {+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    v[6] = {+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    v[7] = {-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    // Top
    v[8] = {-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    v[9] = {-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    v[10] = {+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    v[11] = {+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f};
    // Bottom
    v[12] = {-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f};
    v[13] = {+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    v[14] = {+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    v[15] = {-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    // Left
    v[16] = {-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f};
    v[17] = {-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f};
    v[18] = {-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f};
    v[19] = {-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f};
    // Right
    v[20] = {+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f};
    v[21] = {+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
    v[22] = {+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f};
    v[23] = {+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

    // Front
    i[0] = 0;
    i[1] = 1;
    i[2] = 2;
    i[3] = 0;
    i[4] = 2;
    i[5] = 3;
    // Back
    i[6] = 4;
    i[7] = 5;
    i[8] = 6;
    i[9] = 4;
    i[10] = 6;
    i[11] = 7;
    // Top
    i[12] = 8;
    i[13] = 9;
    i[14] = 10;
    i[15] = 8;
    i[16] = 10;
    i[17] = 11;
    // Bottom
    i[18] = 12;
    i[19] = 13;
    i[20] = 14;
    i[21] = 12;
    i[22] = 14;
    i[23] = 15;
    // Left
    i[24] = 16;
    i[25] = 17;
    i[26] = 18;
    i[27] = 16;
    i[28] = 18;
    i[29] = 19;
    // Right
    i[30] = 20;
    i[31] = 21;
    i[32] = 22;
    i[33] = 20;
    i[34] = 22;
    i[35] = 23;

    subdivisions = subdivisions > 6 ? 6 : subdivisions;
    for (int32 s = 0; s < subdivisions; ++s)
        Subdivide(meshData);

    return meshData;
}

MeshData MeshGenerator::CreateSphere(float radius, int32 subdivisions)
{
    MeshData meshData;

    const float X = 0.525731f;
    const float Z = 0.850651f;

    Vector3 pos[12] = {
        {-X, 0.0f, Z},
        {X, 0.0f, Z},
        {-X, 0.0f, -Z},
        {X, 0.0f, -Z},
        {0.0f, Z, X},
        {0.0f, Z, -X},
        {0.0f, -Z, X},
        {0.0f, -Z, -X},
        {Z, X, 0.0f},
        {-Z, X, 0.0f},
        {Z, -X, 0.0f},
        {-Z, -X, 0.0f}};

    uint32 k[60] = {
        1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4,
        1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2,
        3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0,
        10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7};

    meshData.vertices.SetCount(12);
    meshData.indices.SetCount(60);

    for (int32 i = 0; i < 12; ++i)
        meshData.vertices[i].position = pos[i];
    for (int32 i = 0; i < 60; ++i)
        meshData.indices[i] = k[i];

    subdivisions = subdivisions > 6 ? 6 : subdivisions;
    for (int32 s = 0; s < subdivisions; ++s)
        Subdivide(meshData);
    
    for (int32 i = 0; i < meshData.vertices.Count(); ++i)
    {
        Vector3 n = Vector3(meshData.vertices[i].position).Normalize();
        Vector3 p = radius * n;

        meshData.vertices[i].position = p;
        meshData.vertices[i].normal = n;

        float theta = Math::Atan2(meshData.vertices[i].position.z, meshData.vertices[i].position.x);
        if (theta < 0.0f) theta += Math::TWO_PI;
        float phi = Math::Acos(meshData.vertices[i].normal.y);

        meshData.vertices[i].uv.x = theta / Math::TWO_PI;
        meshData.vertices[i].uv.y = phi / Math::PI;
        meshData.vertices[i].tangent.x = -radius * Math::Sin(phi) * Math::Sin(theta);
        meshData.vertices[i].tangent.y = 0.0f;
        meshData.vertices[i].tangent.z = radius * Math::Sin(phi) * Math::Cos(theta);
        meshData.vertices[i].tangent.Normalize();
    }

    return meshData;
}