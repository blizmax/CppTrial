#pragma once

#include "Math/.Package.h"
#include "Math/Matrix3.h"
#include "Math/Vector3.h"
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

    float Determinant() const;
    Matrix4 Transpose() const;
    Matrix4 Inverse() const;
    Matrix4 Adjugate() const;
    String ToString() const;

    //TODO Rotate
    static Matrix4 Scale(const Vector3 &v);
    static Matrix4 Scale(float x, float y, float z);
    static Matrix4 Translate(const Vector3 &v);
    static Matrix4 Translate(float x, float y, float z);
    static Matrix4 Ortho(float left, float right, float bottom, float top, float near, float far);
    static Matrix4 Projection(float left, float right, float bottom, float top, float near, float far);
    static Matrix4 Projection(float fovY, float aspectRatio, float near, float far);
    static Matrix4 LookAt(const Vector3 &direction, const Vector3 &up);
    static Matrix4 LookAt(const Vector3 &position, const Vector3 &target, const Vector3 &up);

    Matrix4 &SetIdentity()
    {
        v[0][0] = v[1][1] = v[2][2] = v[3][3] = 1.0f;
        v[1][0] = v[2][0] = v[3][0] = v[0][1] = v[2][1] = v[3][1] = v[0][2] = v[1][2] = v[3][2] = v[0][3] = v[1][3] = v[2][3] = 0.0f;
        return *this;
    }

    float *GetPtr()
    {
        return &(v[0][0]);
    }

    const float *GetPtr() const
    {
        return &(v[0][0]);
    }

    float &operator()(uint32 r, uint32 c)
    {
        return v[c][r];
    }

    float operator()(uint32 r, uint32 c) const
    {
        return v[c][r];
    }

    bool operator==(const Matrix4 &rhs) const
    {
        for (uint32 i = 0; i < 4; ++i)
        {
            for (uint32 j = 0; j < 4; ++j)
            {
                if (v[i][j] != rhs.v[i][j])
                    return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix4 &rhs) const
    {
        return !(*this == rhs);
    }

    Matrix4 &operator*=(const Matrix4 &rhs)
    {
        Matrix4 temp = *this * rhs;
        *this = temp;
        return *this;
    }

    Matrix4 &operator*=(float rhs)
    {
        for (uint32 col = 0; col < 4; ++col)
        {
            for (uint32 row = 0; row < 4; ++row)
            {
                v[col][row] *= rhs;
            }
        }
        return *this;
    }

    Matrix4 &operator/=(float rhs)
    {
        for (uint32 col = 0; col < 4; ++col)
        {
            for (uint32 row = 0; row < 4; ++row)
            {
                v[col][row] /= rhs;
            }
        }
        return *this;
    }

    Matrix4 operator+(const Matrix4 &rhs)
    {
        Matrix4 ret;
        for (uint32 col = 0; col < 4; ++col)
        {
            for (uint32 row = 0; row < 4; ++row)
            {
                ret.v[col][row] = v[col][row] + rhs.v[col][row];
            }
        }
        return ret;
    }

    Matrix4 operator-(const Matrix4 &rhs)
    {
        Matrix4 ret;
        for (uint32 col = 0; col < 4; ++col)
        {
            for (uint32 row = 0; row < 4; ++row)
            {
                ret.v[col][row] = v[col][row] - rhs.v[col][row];
            }
        }
        return ret;
    }

    Matrix4 operator*(const Matrix4 &rhs)
    {
        Matrix4 ret;
        for (uint32 col = 0; col < 4; ++col)
        {
            for (uint32 row = 0; row < 4; ++row)
            {
                ret.v[col][row] =
                    v[0][row] * rhs.v[col][0] + v[1][row] * rhs.v[col][1] + v[2][row] * rhs.v[col][2] + v[3][row] * rhs.v[col][3];
            }
        }
        return ret;
    }

    friend Matrix4 operator*(const Matrix4 &lhs, float rhs)
    {
        Matrix4 ret = lhs;
        ret *= rhs;
        return ret;
    }

    friend Matrix4 operator*(float lhs, const Matrix4 &rhs)
    {
        Matrix4 ret = rhs;
        ret *= lhs;
        return ret;
    }

    friend Vector4 operator*(const Vector4 &lhs, const Matrix4 &rhs)
    {
        Vector4 ret;
        for (uint32 col = 0; col < 4; ++col)
        {
            ret[col] =
                rhs.v[col][0] * lhs[0] + rhs.v[col][1] * lhs[1] + rhs.v[col][2] * lhs[2] + rhs.v[col][3] * lhs[3];
        }
        return ret;
    }

    friend Vector4 operator*(const Matrix4 &lhs, const Vector4 &rhs)
    {
        Vector4 ret;
        for (uint32 row = 0; row < 4; ++row)
        {
            ret[row] =
                lhs.v[0][row] * rhs[0] + lhs.v[1][row] * rhs[1] + lhs.v[2][row] * rhs[2] + lhs.v[3][row] * rhs[3];
        }
        return ret;
    }

    // Assume w component is 1
    friend Vector3 operator*(const Vector3 &lhs, const Matrix4 &rhs)
    {
        Vector4 temp = Vector4(lhs.x, lhs.y, lhs.z, 1.0f) * rhs;
        return Vector3(temp.x, temp.y, temp.z) / temp.w;
    }

    // Assume w component is 1
    friend Vector3 operator*(const Matrix4 &lhs, const Vector3 &rhs)
    {
        Vector4 temp = lhs * Vector4(rhs.x, rhs.y, rhs.z, 1.0f);
        return Vector3(temp.x, temp.y, temp.z) / temp.w;
    }

private:
    float v[4][4];
};