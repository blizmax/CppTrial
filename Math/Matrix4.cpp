#include "Math/Matrix4.h"

Matrix4 &Matrix4::SetRow(uint32 i, const Vector4 &row)
{
    CT_ASSERT(i < 4);

    v[0][i] = row.x;
    v[1][i] = row.y;
    v[2][i] = row.z;
    v[3][i] = row.w;
    return *this;
}

Vector4 Matrix4::GetRow(uint32 i) const
{
    CT_ASSERT(i < 4);

    return Vector4(v[0][i], v[1][i], v[2][i], v[3][i]);
}

Matrix4 &Matrix4::SetColumn(uint32 i, const Vector4 &col)
{
    CT_ASSERT(i < 4);

    v[i][0] = col.x;
    v[i][1] = col.y;
    v[i][2] = col.z;
    v[i][3] = col.w;
    return *this;
}

Vector4 Matrix4::GetColumn(uint32 i) const
{
    CT_ASSERT(i < 4);

    return Vector4(v[i][0], v[i][1], v[i][2], v[i][3]);
}

float Matrix4::Determinant() const
{
    float n00 = v[0][0];
    float n01 = v[1][0];
    float n02 = v[2][0];
    float n03 = v[3][0];
    float n10 = v[0][1];
    float n11 = v[1][1];
    float n12 = v[2][1];
    float n13 = v[3][1];
    float n20 = v[0][2];
    float n21 = v[1][2];
    float n22 = v[2][2];
    float n23 = v[3][2];
    float n30 = v[0][3];
    float n31 = v[1][3];
    float n32 = v[2][3];
    float n33 = v[3][3];

    return (n00 * (n11 * (n22 * n33 - n23 * n32) + n12 * (n23 * n31 - n21 * n33) + n13 * (n21 * n32 - n22 * n31)) +
            n01 * (n10 * (n23 * n32 - n22 * n33) + n12 * (n20 * n33 - n23 * n30) + n13 * (n22 * n30 - n20 * n32)) +
            n02 * (n10 * (n21 * n33 - n23 * n31) + n11 * (n23 * n30 - n20 * n33) + n13 * (n20 * n31 - n21 * n30)) +
            n03 * (n10 * (n22 * n31 - n21 * n32) + n11 * (n20 * n32 - n22 * n30) + n12 * (n21 * n30 - n20 * n31)));
}

Matrix4 Matrix4::Transpose() const
{
    Matrix4 ret;
    for (uint32 col = 0; col < 4; ++col)
    {
        for (uint32 row = 0; row < 4; ++row)
        {
            ret.v[col][row] = v[row][col];
        }
    }
    return ret;
}

Matrix4 Matrix4::Inverse() const
{
    float n00 = v[0][0];
    float n01 = v[1][0];
    float n02 = v[2][0];
    float n03 = v[3][0];
    float n10 = v[0][1];
    float n11 = v[1][1];
    float n12 = v[2][1];
    float n13 = v[3][1];
    float n20 = v[0][2];
    float n21 = v[1][2];
    float n22 = v[2][2];
    float n23 = v[3][2];
    float n30 = v[0][3];
    float n31 = v[1][3];
    float n32 = v[2][3];
    float n33 = v[3][3];

    float p00 = n11 * (n22 * n33 - n23 * n32) + n12 * (n23 * n31 - n21 * n33) + n13 * (n21 * n32 - n22 * n31);
    float p10 = n10 * (n23 * n32 - n22 * n33) + n12 * (n20 * n33 - n23 * n30) + n13 * (n22 * n30 - n20 * n32);
    float p20 = n10 * (n21 * n33 - n23 * n31) + n11 * (n23 * n30 - n20 * n33) + n13 * (n20 * n31 - n21 * n30);
    float p30 = n10 * (n22 * n31 - n21 * n32) + n11 * (n20 * n32 - n22 * n30) + n12 * (n21 * n30 - n20 * n31);

    float t = 1.0F / (n00 * p00 + n01 * p10 + n02 * p20 + n03 * p30);

    return Matrix4(p00 * t,
                   (n01 * (n23 * n32 - n22 * n33) + n02 * (n21 * n33 - n23 * n31) + n03 * (n22 * n31 - n21 * n32)) * t,
                   (n01 * (n12 * n33 - n13 * n32) + n02 * (n13 * n31 - n11 * n33) + n03 * (n11 * n32 - n12 * n31)) * t,
                   (n01 * (n13 * n22 - n12 * n23) + n02 * (n11 * n23 - n13 * n21) + n03 * (n12 * n21 - n11 * n22)) * t,
                   p10 * t,
                   (n00 * (n22 * n33 - n23 * n32) + n02 * (n23 * n30 - n20 * n33) + n03 * (n20 * n32 - n22 * n30)) * t,
                   (n00 * (n13 * n32 - n12 * n33) + n02 * (n10 * n33 - n13 * n30) + n03 * (n12 * n30 - n10 * n32)) * t,
                   (n00 * (n12 * n23 - n13 * n22) + n02 * (n13 * n20 - n10 * n23) + n03 * (n10 * n22 - n12 * n20)) * t,
                   p20 * t,
                   (n00 * (n23 * n31 - n21 * n33) + n01 * (n20 * n33 - n23 * n30) + n03 * (n21 * n30 - n20 * n31)) * t,
                   (n00 * (n11 * n33 - n13 * n31) + n01 * (n13 * n30 - n10 * n33) + n03 * (n10 * n31 - n11 * n30)) * t,
                   (n00 * (n13 * n21 - n11 * n23) + n01 * (n10 * n23 - n13 * n20) + n03 * (n11 * n20 - n10 * n21)) * t,
                   p30 * t,
                   (n00 * (n21 * n32 - n22 * n31) + n01 * (n22 * n30 - n20 * n32) + n02 * (n20 * n31 - n21 * n30)) * t,
                   (n00 * (n12 * n31 - n11 * n32) + n01 * (n10 * n32 - n12 * n30) + n02 * (n11 * n30 - n10 * n31)) * t,
                   (n00 * (n11 * n22 - n12 * n21) + n01 * (n12 * n20 - n10 * n22) + n02 * (n10 * n21 - n11 * n20)) * t);
}

Matrix4 Matrix4::Adjugate() const
{
    float n00 = v[0][0];
    float n01 = v[1][0];
    float n02 = v[2][0];
    float n03 = v[3][0];
    float n10 = v[0][1];
    float n11 = v[1][1];
    float n12 = v[2][1];
    float n13 = v[3][1];
    float n20 = v[0][2];
    float n21 = v[1][2];
    float n22 = v[2][2];
    float n23 = v[3][2];
    float n30 = v[0][3];
    float n31 = v[1][3];
    float n32 = v[2][3];
    float n33 = v[3][3];

    return Matrix4(n11 * (n22 * n33 - n23 * n32) + n12 * (n23 * n31 - n21 * n33) + n13 * (n21 * n32 - n22 * n31),
                   n01 * (n23 * n32 - n22 * n33) + n02 * (n21 * n33 - n23 * n31) + n03 * (n22 * n31 - n21 * n32),
                   n01 * (n12 * n33 - n13 * n32) + n02 * (n13 * n31 - n11 * n33) + n03 * (n11 * n32 - n12 * n31),
                   n01 * (n13 * n22 - n12 * n23) + n02 * (n11 * n23 - n13 * n21) + n03 * (n12 * n21 - n11 * n22),
                   n10 * (n23 * n32 - n22 * n33) + n12 * (n20 * n33 - n23 * n30) + n13 * (n22 * n30 - n20 * n32),
                   n00 * (n22 * n33 - n23 * n32) + n02 * (n23 * n30 - n20 * n33) + n03 * (n20 * n32 - n22 * n30),
                   n00 * (n13 * n32 - n12 * n33) + n02 * (n10 * n33 - n13 * n30) + n03 * (n12 * n30 - n10 * n32),
                   n00 * (n12 * n23 - n13 * n22) + n02 * (n13 * n20 - n10 * n23) + n03 * (n10 * n22 - n12 * n20),
                   n10 * (n21 * n33 - n23 * n31) + n11 * (n23 * n30 - n20 * n33) + n13 * (n20 * n31 - n21 * n30),
                   n00 * (n23 * n31 - n21 * n33) + n01 * (n20 * n33 - n23 * n30) + n03 * (n21 * n30 - n20 * n31),
                   n00 * (n11 * n33 - n13 * n31) + n01 * (n13 * n30 - n10 * n33) + n03 * (n10 * n31 - n11 * n30),
                   n00 * (n13 * n21 - n11 * n23) + n01 * (n10 * n23 - n13 * n20) + n03 * (n11 * n20 - n10 * n21),
                   n10 * (n22 * n31 - n21 * n32) + n11 * (n20 * n32 - n22 * n30) + n12 * (n21 * n30 - n20 * n31),
                   n00 * (n21 * n32 - n22 * n31) + n01 * (n22 * n30 - n20 * n32) + n02 * (n20 * n31 - n21 * n30),
                   n00 * (n12 * n31 - n11 * n32) + n01 * (n10 * n32 - n12 * n30) + n02 * (n11 * n30 - n10 * n31),
                   n00 * (n11 * n22 - n12 * n21) + n01 * (n12 * n20 - n10 * n22) + n02 * (n10 * n21 - n11 * n20));
}

String Matrix4::ToString() const
{
    return String::Format(CT_TEXT("[{0},{1},{2},{3}|{4},{5},{6},{7}|{8},{9},{10},{11}|{12},{13},{14},{15}]"),
                          v[0][0], v[1][0], v[2][0], v[3][0], v[0][1], v[1][1], v[2][1], v[3][1],
                          v[0][2], v[1][2], v[2][2], v[3][2], v[0][3], v[1][3], v[2][3], v[3][3]);
}