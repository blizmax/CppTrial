#include "Math/Matrix4.h"
#include "Math/Quat.h"

Matrix4 &Matrix4::SetRow(int32 i, const Vector4 &row)
{
    CT_CHECK(i >= 0 && i < 4);

    v[0][i] = row.x;
    v[1][i] = row.y;
    v[2][i] = row.z;
    v[3][i] = row.w;
    return *this;
}

Vector4 Matrix4::GetRow(int32 i) const
{
    CT_CHECK(i >= 0 && i < 4);

    return Vector4(v[0][i], v[1][i], v[2][i], v[3][i]);
}

Matrix4 &Matrix4::SetColumn(int32 i, const Vector4 &col)
{
    CT_CHECK(i >= 0 && i < 4);

    v[i][0] = col.x;
    v[i][1] = col.y;
    v[i][2] = col.z;
    v[i][3] = col.w;
    return *this;
}

Vector4 Matrix4::GetColumn(int32 i) const
{
    CT_CHECK(i >= 0 && i < 4);

    return Vector4(v[i][0], v[i][1], v[i][2], v[i][3]);
}

Matrix3 Matrix4::ToMatrix3() const
{
    Matrix3 m;
    m.v[0][0] = v[0][0];
    m.v[0][1] = v[0][1];
    m.v[0][2] = v[0][2];
    m.v[1][0] = v[1][0];
    m.v[1][1] = v[1][1];
    m.v[1][2] = v[1][2];
    m.v[2][0] = v[2][0];
    m.v[2][1] = v[2][1];
    m.v[2][2] = v[2][2];
    return m;
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

    return n00 * (n11 * (n22 * n33 - n23 * n32) + n12 * (n23 * n31 - n21 * n33) + n13 * (n21 * n32 - n22 * n31)) +
           n01 * (n10 * (n23 * n32 - n22 * n33) + n12 * (n20 * n33 - n23 * n30) + n13 * (n22 * n30 - n20 * n32)) +
           n02 * (n10 * (n21 * n33 - n23 * n31) + n11 * (n23 * n30 - n20 * n33) + n13 * (n20 * n31 - n21 * n30)) +
           n03 * (n10 * (n22 * n31 - n21 * n32) + n11 * (n20 * n32 - n22 * n30) + n12 * (n21 * n30 - n20 * n31));
}

Matrix4 Matrix4::Transpose() const
{
    Matrix4 ret;
    for (int32 col = 0; col < 4; ++col)
    {
        for (int32 row = 0; row < 4; ++row)
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

    float t = 1.0f / (n00 * p00 + n01 * p10 + n02 * p20 + n03 * p30);

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

Matrix4 Matrix4::Rotate(const Vector3 &axis, float rad)
{
    Quat quat = Quat(axis, rad);
    return quat.ToMatrix4();
}

Matrix4 Matrix4::Rotate(float yaw, float pitch, float roll)
{
    Quat quat = Quat(yaw, pitch, roll);
    return quat.ToMatrix4();
}

Matrix4 Matrix4::Rotate(const Quat &quat)
{
    return quat.ToMatrix4();
}

Matrix4 Matrix4::Scale(const Vector3 &v)
{
    Matrix4 m = Matrix4();
    m.v[0][0] = v.x;
    m.v[1][1] = v.y;
    m.v[2][2] = v.z;
    m.v[3][3] = 1.0f;
    return m;
}

Matrix4 Matrix4::Scale(float x, float y, float z)
{
    Matrix4 m = Matrix4();
    m.v[0][0] = x;
    m.v[1][1] = y;
    m.v[2][2] = z;
    m.v[3][3] = 1.0f;
    return m;
}

Matrix4 Matrix4::Translate(const Vector3 &v)
{
    Matrix4 m = Matrix4();
    m.v[3][0] = v.x;
    m.v[3][1] = v.y;
    m.v[3][2] = v.z;
    m.v[3][3] = 1.0f;
    return m;
}

Matrix4 Matrix4::Translate(float x, float y, float z)
{
    Matrix4 m = Matrix4();
    m.v[3][0] = x;
    m.v[3][1] = y;
    m.v[3][2] = z;
    m.v[3][3] = 1.0f;
    return m;
}

Matrix4 Matrix4::TRS(const Vector3 &translation, const Quat &rotation, const Vector3 &scale)
{
    Matrix4 m = Matrix4();
    const float xs = rotation.x * 2.0f, ys = rotation.y * 2.0f, zs = rotation.z * 2.0f;
    const float wx = rotation.w * xs, wy = rotation.w * ys, wz = rotation.w * zs;
    const float xx = rotation.x * xs, xy = rotation.x * ys, xz = rotation.x * zs;
    const float yy = rotation.y * ys, yz = rotation.y * zs, zz = rotation.z * zs;

    m.v[0][0] = scale.x * (1.0f - (yy + zz));
    m.v[1][0] = scale.y * (xy - wz);
    m.v[2][0] = scale.z * (xz + wy);
    m.v[3][0] = translation.x;

    m.v[0][1] = scale.x * (xy + wz);
    m.v[1][1] = scale.y * (1.0f - (xx + zz));
    m.v[2][1] = scale.z * (yz - wx);
    m.v[3][1] = translation.y;

    m.v[0][2] = scale.x * (xz - wy);
    m.v[1][2] = scale.y * (yz + wx);
    m.v[2][2] = scale.z * (1.0f - (xx + yy));
    m.v[3][2] = translation.z;

    m.v[0][3] = 0.0f;
    m.v[1][3] = 0.0f;
    m.v[2][3] = 0.0f;
    m.v[3][3] = 1.0f;
    return m;
}

Matrix4 Matrix4::Ortho(float left, float right, float bottom, float top, float n, float f)
{
    Matrix4 m = Matrix4();
    float xOrth = 2.0f / (right - left);
    float yOrth = 2.0f / (top - bottom);
    float zOrth = -2.0f / (f - n);

    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    float tz = -(f + n) / (f - n);

    m.v[0][0] = xOrth;
    m.v[0][1] = 0.0f;
    m.v[0][2] = 0.0f;
    m.v[0][3] = 0.0f;
    m.v[1][0] = 0.0f;
    m.v[1][1] = yOrth;
    m.v[1][2] = 0.0f;
    m.v[1][3] = 0.0f;
    m.v[2][0] = 0.0f;
    m.v[2][1] = 0.0f;
    m.v[2][2] = zOrth;
    m.v[2][3] = 0.0f;
    m.v[3][0] = tx;
    m.v[3][1] = ty;
    m.v[3][2] = tz;
    m.v[3][3] = 1.0f;
    return m;
}

Matrix4 Matrix4::Projection(float left, float right, float bottom, float top, float n, float f)
{
    Matrix4 m = Matrix4();

    float x = 2.0f * n / (right - left);
    float y = 2.0f * n / (top - bottom);
    float a = (right + left) / (right - left);
    float b = (top + bottom) / (top - bottom);
    float l_a1 = (f + n) / (n - f);
    float l_a2 = (2.0f * f * n) / (n - f);

    m.v[0][0] = x;
    m.v[0][1] = 0.0f;
    m.v[0][2] = 0.0f;
    m.v[0][3] = 0.0f;
    m.v[1][0] = 0.0f;
    m.v[1][1] = y;
    m.v[1][2] = 0.0f;
    m.v[1][3] = 0.0f;
    m.v[2][0] = a;
    m.v[2][1] = b;
    m.v[2][2] = l_a1;
    m.v[2][3] = -1.0f;
    m.v[3][0] = 0.0f;
    m.v[3][1] = 0.0f;
    m.v[3][2] = l_a2;
    m.v[3][3] = 0.0f;
    return m;
}

Matrix4 Matrix4::Projection(float fovY, float aspect, float n, float f)
{
    Matrix4 m = Matrix4();
    float l_fd = 1.0f / Math::Tan((fovY * Math::DEG_TO_RAD) / 2.0f);
    float l_a1 = (f + n) / (n - f);
    float l_a2 = (2.0f * f * n) / (n - f);

    m.v[0][0] = l_fd / aspect;
    m.v[0][1] = 0.0f;
    m.v[0][2] = 0.0f;
    m.v[0][3] = 0.0f;
    m.v[1][0] = 0.0f;
    m.v[1][1] = l_fd;
    m.v[1][2] = 0.0f;
    m.v[1][3] = 0.0f;
    m.v[2][0] = 0.0f;
    m.v[2][1] = 0.0f;
    m.v[2][2] = l_a1;
    m.v[2][3] = -1.0f;
    m.v[3][0] = 0.0f;
    m.v[3][1] = 0.0f;
    m.v[3][2] = l_a2;
    m.v[3][3] = 0.0f;
    return m;
}

Matrix4 Matrix4::LookAt(const Vector3 &direction, const Vector3 &up)
{
    Matrix4 m = Matrix4();

    Vector3 vez = Vector3(direction).Normalize();
    Vector3 vex = vez.Cross(up).Normalize();
    Vector3 vey = vex.Cross(vez).Normalize();

    m.v[0][0] = vex.x;
    m.v[1][0] = vex.y;
    m.v[2][0] = vex.z;
    m.v[0][1] = vey.x;
    m.v[1][1] = vey.y;
    m.v[2][1] = vey.z;
    m.v[0][2] = -vez.x;
    m.v[1][2] = -vez.y;
    m.v[2][2] = -vez.z;
    return m;
}

Matrix4 Matrix4::LookAt(const Vector3 &position, const Vector3 &target, const Vector3 &up)
{
    Vector3 direction = target - position;
    Matrix4 m1 = LookAt(direction, up);
    Matrix4 m2 = Translate(-position);
    return m1 * m2;
}