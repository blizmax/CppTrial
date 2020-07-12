#pragma once

#include "Render/.Package.h"

struct MeshData
{
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector3 tangent;
        Vector2 uv;

        Vertex() = default;
        Vertex(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty, float tz, float u, float v)
            : position(px, py, pz), normal(nx, ny, nz), tangent(tx, ty, tz), uv(u, v)
        {
        }
    };

    Array<Vertex> vertices;
    Array<uint32> indices;
};

class MeshGenerator
{
public:
    MeshData CreateBox(float width, float height, float depth, int32 subdivisions);
    MeshData CreateSphere(float radius, int32 subdivisions);
   
private:
    void Subdivide(MeshData &meshData);
};