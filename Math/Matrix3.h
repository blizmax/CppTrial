#pragma once

#include "Math/.Package.h"
#include "Math/Vector3.h"

//Column major order.
class Matrix3
{
public:
    friend class Matrix4;

    Matrix3(const Matrix3 &) = default;
    Matrix3 &operator=(const Matrix3 &) = default;

    Matrix3()
        : v{{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    {
    }

    Matrix3(float n00, float n01, float n02, float n10, float n11, float n12, float n20, float n21, float n22)
        : v{{n00, n10, n20}, {n01, n11, n21}, {n02, n12, n22}}
    {
    }

    Matrix3 &SetRow(uint32 i, const Vector3 &row);
    Vector3 GetRow(uint32 i) const;
    Matrix3 &SetColumn(uint32 i, const Vector3 &col);
    Vector3 GetColumn(uint32 i) const;

    float Determinant() const;
    Matrix3 Transpose() const;
    Matrix3 Inverse() const;
    Matrix3 Adjugate() const;
    String ToString() const;

    Matrix3 &SetIdentity()
    {
        v[0][0] = v[1][1] = v[2][2] = 1.0f;
        v[0][1] = v[0][2] = v[1][0] = v[1][2] = v[2][0] = v[2][1] = 0.0f;
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

    bool operator==(const Matrix3 &rhs) const
    {
        for (uint32 i = 0; i < 3; ++i)
        {
            for (uint32 j = 0; j < 3; ++j)
            {
                if (v[i][j] != rhs.v[i][j])
                    return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix3 &rhs) const
    {
        return !(*this == rhs);
    }

    Matrix3 &operator*=(const Matrix3 &rhs)
    {
        Matrix3 temp = *this * rhs;
        *this = temp;
        return *this;
    }

    Matrix3 &operator*=(float rhs)
    {
        for (uint32 col = 0; col < 3; ++col)
        {
            for (uint32 row = 0; row < 3; ++row)
            {
                v[col][row] *= rhs;
            }
        }
        return *this;
    }

    Matrix3 &operator/=(float rhs)
    {
        for (uint32 col = 0; col < 3; ++col)
        {
            for (uint32 row = 0; row < 3; ++row)
            {
                v[col][row] /= rhs;
            }
        }
        return *this;
    }

    Matrix3 operator+(const Matrix3 &rhs)
    {
        Matrix3 ret;
        for (uint32 col = 0; col < 3; ++col)
        {
            for (uint32 row = 0; row < 3; ++row)
            {
                ret.v[col][row] = v[col][row] + rhs.v[col][row];
            }
        }
        return ret;
    }

    Matrix3 operator-(const Matrix3 &rhs)
    {
        Matrix3 ret;
        for (uint32 col = 0; col < 3; ++col)
        {
            for (uint32 row = 0; row < 3; ++row)
            {
                ret.v[col][row] = v[col][row] - rhs.v[col][row];
            }
        }
        return ret;
    }

    Matrix3 operator*(const Matrix3 &rhs)
    {
        Matrix3 ret;
        for (uint32 col = 0; col < 3; ++col)
        {
            for (uint32 row = 0; row < 3; ++row)
            {
                ret.v[col][row] =
                    v[0][row] * rhs.v[col][0] + v[1][row] * rhs.v[col][1] + v[2][row] * rhs.v[col][2];
            }
        }
        return ret;
    }

    friend Matrix3 operator*(const Matrix3 &lhs, float rhs)
    {
        Matrix3 ret = lhs;
        ret *= rhs;
        return ret;
    }

    friend Matrix3 operator*(float lhs, const Matrix3 &rhs)
    {
        Matrix3 ret = rhs;
        ret *= lhs;
        return ret;
    }

    friend Vector3 operator*(const Vector3 &lhs, const Matrix3 &rhs)
    {
        Vector3 ret;
        for (uint32 col = 0; col < 3; ++col)
        {
            ret[col] =
                rhs.v[col][0] * lhs[0] + rhs.v[col][1] * lhs[1] + rhs.v[col][2] * lhs[2];
        }
        return ret;
    }

    friend Vector3 operator*(const Matrix3 &lhs, const Vector3 &rhs)
    {
        Vector3 ret;
        for (uint32 row = 0; row < 3; ++row)
        {
            ret[row] =
                lhs.v[0][row] * rhs[0] + lhs.v[1][row] * rhs[1] + lhs.v[2][row] * rhs[2];
        }
        return ret;
    }

private:
    float v[3][3];
};
