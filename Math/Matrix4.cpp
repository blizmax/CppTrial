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