#include "Math/Matrix3.h"

Matrix3& Matrix3::SetRow(uint32 i, const Vector3& row)
{
    CT_ASSERT(i < 3);

    v[0][i] = row.x;
    v[1][i] = row.y;
    v[2][i] = row.z;
    return *this;
}

Vector3 Matrix3::GetRow(uint32 i) const
{
    CT_ASSERT(i < 3);

    return Vector3(v[0][i], v[1][i], v[2][i]);
}

Matrix3& Matrix3::SetColumn(uint32 i, const Vector3& col)
{
    CT_ASSERT(i < 3);

    v[i][0] = col.x;
    v[i][1] = col.y;
    v[i][2] = col.z;
    return *this;
}

Vector3 Matrix3::GetColumn(uint32 i) const
{
    CT_ASSERT(i < 3);

    return Vector3(v[i][0], v[i][1], v[i][2]);
}