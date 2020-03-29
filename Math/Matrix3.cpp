#include "Math/Matrix3.h"

Matrix3 &Matrix3::SetRow(int32 i, const Vector3 &row)
{
    CT_CHECK(i >= 0 && i < 3);

    v[0][i] = row.x;
    v[1][i] = row.y;
    v[2][i] = row.z;
    return *this;
}

Vector3 Matrix3::GetRow(int32 i) const
{
    CT_CHECK(i >= 0 && i < 3);

    return Vector3(v[0][i], v[1][i], v[2][i]);
}

Matrix3 &Matrix3::SetColumn(int32 i, const Vector3 &col)
{
    CT_CHECK(i >= 0 && i < 3);

    v[i][0] = col.x;
    v[i][1] = col.y;
    v[i][2] = col.z;
    return *this;
}

Vector3 Matrix3::GetColumn(int32 i) const
{
    CT_CHECK(i >= 0 && i < 3);

    return Vector3(v[i][0], v[i][1], v[i][2]);
}

float Matrix3::Determinant() const
{
    return v[0][0] * (v[1][1] * v[2][2] - v[2][1] * v[1][2]) +
           v[1][0] * (v[2][1] * v[0][2] - v[0][1] * v[2][2]) +
           v[2][0] * (v[0][1] * v[1][2] - v[1][1] * v[0][2]);
}

Matrix3 Matrix3::Transpose() const
{
    Matrix3 ret;
    for (int32 col = 0; col < 3; ++col)
    {
        for (int32 row = 0; row < 3; ++row)
        {
            ret.v[col][row] = v[row][col];
        }
    }
    return ret;
}

Matrix3 Matrix3::Inverse() const
{
    float n00 = v[0][0];
    float n01 = v[1][0];
    float n02 = v[2][0];
    float n10 = v[0][1];
    float n11 = v[1][1];
    float n12 = v[2][1];
    float n20 = v[0][2];
    float n21 = v[1][2];
    float n22 = v[2][2];

    float p00 = n11 * n22 - n12 * n21;
    float p10 = n12 * n20 - n10 * n22;
    float p20 = n10 * n21 - n11 * n20;

    float t = 1.0f / (n00 * p00 + n01 * p10 + n02 * p20);

    return Matrix3(p00 * t, (n02 * n21 - n01 * n22) * t, (n01 * n12 - n02 * n11) * t,
                   p10 * t, (n00 * n22 - n02 * n20) * t, (n02 * n10 - n00 * n12) * t,
                   p20 * t, (n01 * n20 - n00 * n21) * t, (n00 * n11 - n01 * n10) * t);
}

Matrix3 Matrix3::Adjugate() const
{
    float n00 = v[0][0];
    float n01 = v[1][0];
    float n02 = v[2][0];
    float n10 = v[0][1];
    float n11 = v[1][1];
    float n12 = v[2][1];
    float n20 = v[0][2];
    float n21 = v[1][2];
    float n22 = v[2][2];

    return Matrix3(n11 * n22 - n12 * n21, n02 * n21 - n01 * n22, n01 * n12 - n02 * n11,
                   n12 * n20 - n10 * n22, n00 * n22 - n02 * n20, n02 * n10 - n00 * n12,
                   n10 * n21 - n11 * n20, n01 * n20 - n00 * n21, n00 * n11 - n01 * n10);
}

String Matrix3::ToString() const
{
    return String::Format(CT_TEXT("[{0},{1},{2}|{3},{4},{5}|{6},{7},{8}]"),
                          v[0][0], v[1][0], v[2][0], v[0][1], v[1][1], v[2][1], v[0][2], v[1][2], v[2][2]);
}