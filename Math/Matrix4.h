#pragma once

#include "Math/.Package.h"
#include "Math/Matrix3.h"
#include "Math/Vector4.h"

class Matrix4
{
public:
    Matrix4(const Matrix4 &) = default;
    Matrix4 &operator=(const Matrix4 &) = default;

    Matrix4()
        : v{
              {1.0f, 0.0f, 0.0f, 0.0f},
              {0.0f, 1.0f, 0.0f, 0.0f},
              {0.0f, 0.0f, 1.0f, 0.0f},
              {0.0f, 0.0f, 0.0f, 1.0f},
          }
    {
    }

    Matrix4(float n00, float n01, float n02, float n03,
            float n10, float n11, float n12, float n13,
            float n20, float n21, float n22, float n23,
            float n30, float n31, float n32, float n33)
        : v{
              {n00, n10, n20, n30},
              {n01, n11, n21, n31},
              {n02, n12, n22, n32},
              {n03, n13, n23, n33},
          }
    {
    }

    explicit Matrix4(const Matrix3 &m)
        : v{
              {m.v[0][0], m.v[0][1], m.v[0][2], 0.0f},
              {m.v[1][0], m.v[1][1], m.v[1][2], 0.0f},
              {m.v[2][0], m.v[2][1], m.v[2][2], 0.0f},
              {0.0f, 0.0f, 0.0f, 1.0f},
          }
    {
    }

    Matrix4 &SetRow(uint32 i, const Vector4 &row);
    Vector4 GetRow(uint32 i) const;
    Matrix4 &SetColumn(uint32 i, const Vector4 &col);
    Vector4 GetColumn(uint32 i) const;

private:
    float v[4][4];
};