#pragma once

#include "Math/.Package.h"
#include "Math/Vector3.h"

//Column major order.
class Matrix3
{
public:
    Matrix3& SetRow(uint32 i, const Vector3& row);
    Vector3 GetRow(uint32 i) const;
    Matrix3& SetColumn(uint32 i, const Vector3& col);
    Vector3 GetColumn(uint32 i) const;

    float& operator()(uint32 r, uint32 c)
    {
        return v[c][r];
    }

    float operator()(uint32 r, uint32 c) const
    {
        return v[c][r];
    }

    bool operator==(const Matrix3& rhs) const
    {
        for(uint32 i = 0; i < 3; ++i)
        {
            for(uint32 j = 0; j < 3; ++j)
            {
                if(v[i][j] != rhs.v[i][j])
                    return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix3& rhs) const
    {
        return !(*this == rhs);
    }

    Matrix3 &operator*=(const Matrix3 &rhs)
    {
        Matrix3 temp = *this * rhs;
        *this = temp;
        return *this;
    }

    Matrix3 &operator*=(float rhs);
    Matrix3 &operator/=(float rhs);

    Matrix3 operator+(const Matrix3& rhs);
    Matrix3 operator-(const Matrix3& rhs);

    Matrix3 operator*(const Matrix3& rhs)
    {
        Matrix3 ret;
        for (uint32 col = 0; col < 3; ++col)
        {
            for(uint32 row = 0; row < 3; ++row)
            {
                ret.v[col][row] = v[0][row] * rhs.v[col][0] +
                    v[1][row] * rhs.v[col][1] +
                    v[2][row] * rhs.v[col][2];
            }
        }
        return ret;
    }

    friend Matrix3 operator*(const Matrix3& lhs, float rhs);
    friend Matrix3 operator*(float lhs, const Matrix3& rhs);
    friend Matrix3 operator*(const Vector3& lhs, const Matrix3& rhs);
    friend Matrix3 operator*(const Matrix3& lhs, const Vector3& rhs);

private:
    float v[3][3];

};

