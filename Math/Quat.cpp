#include "Math/Quat.h"
#include "Math/Matrix3.h"
#include "Math/Matrix4.h"

Quat &Quat::FromMatrix(const Matrix3 &mat)
{
    Vector3 xAxis = {mat(0, 0), mat(0, 1), mat(0, 2)};
    Vector3 yAxis = {mat(1, 0), mat(1, 1), mat(1, 2)};
    Vector3 zAxis = {mat(2, 0), mat(2, 1), mat(2, 2)};
    return FromAxes(xAxis, yAxis, zAxis);
}

Quat &Quat::FromMatrix(const Matrix4 &mat)
{
    Vector3 xAxis = {mat(0, 0), mat(0, 1), mat(0, 2)};
    Vector3 yAxis = {mat(1, 0), mat(1, 1), mat(1, 2)};
    Vector3 zAxis = {mat(2, 0), mat(2, 1), mat(2, 2)};
    return FromAxes(xAxis, yAxis, zAxis);
}

// Assum axes are normalized.
Quat &Quat::FromAxes(const Vector3 &xAxis, const Vector3 &yAxis, const Vector3 &zAxis)
{
    const float xx = xAxis.x;
    const float xy = xAxis.y;
    const float xz = xAxis.z;
    const float yx = yAxis.x;
    const float yy = yAxis.y;
    const float yz = yAxis.z;
    const float zx = zAxis.x;
    const float zy = zAxis.y;
    const float zz = zAxis.z;
    const float t = xx + yy + zz;

    if (t >= 0)
    {
        float s = Math::Sqrt(t + 1.0f);
        w = 0.5f * s;
        s = 0.5f / s;
        x = (zy - yz) * s;
        y = (xz - zx) * s;
        z = (yx - xy) * s;
    }
    else if ((xx > yy) && (xx > zz))
    {
        float s = Math::Sqrt(1.0f + xx - yy - zz);
        x = s * 0.5f;
        s = 0.5f / s;
        y = (yx + xy) * s;
        z = (xz + zx) * s;
        w = (zy - yz) * s;
    }
    else if (yy > zz)
    {
        float s = Math::Sqrt(1.0f + yy - xx - zz);
        y = s * 0.5f;
        s = 0.5f / s;
        x = (yx + xy) * s;
        z = (zy + yz) * s;
        w = (xz - zx) * s;
    }
    else
    {
        float s = Math::Sqrt(1.0f + zz - xx - yy);
        z = s * 0.5f;
        s = 0.5f / s;
        x = (xz + zx) * s;
        y = (zy + yz) * s;
        w = (yx - xy) * s;
    }
    return *this;
}

Quat &Quat::FromAxisRad(const Vector3 &axis, float rad)
{
    const float half = 0.5f * rad;
    const float sin = Math::Sin(half);
    const float cos = Math::Cos(half);

    x = sin * axis.x;
    y = sin * axis.y;
    z = sin * axis.z;
    w = cos;

    return *this;
}

Quat &Quat::FromEulerRad(float yaw, float pitch, float roll)
{
    const float hr = roll * 0.5f;
    const float shr = Math::Sin(hr);
    const float chr = Math::Cos(hr);
    const float hp = pitch * 0.5f;
    const float shp = Math::Sin(hp);
    const float chp = Math::Cos(hp);
    const float hy = yaw * 0.5f;
    const float shy = Math::Sin(hy);
    const float chy = Math::Cos(hy);

    Quat quatPitch(shp, 0.0f, 0.0f, chp);
    Quat quatYaw(0.0f, shy, 0.0f, chy);
    Quat quatRoll(0.0f, 0.0f, shr, chr);

    //Rotate order : yaw -> pitch -> roll
    *this = quatRoll * (quatPitch * quatYaw);
    return *this;
}

Quat &Quat::SetLookRotation(const Vector3 &forward, const Vector3 &up)
{
    Vector3 f = Vector3(forward).Normalize();
    Vector3 r = f.Cross(up).Normalize();
    Vector3 u = r.Cross(f).Normalize();

    return FromAxes(r, u, f);
}

// Assume from and to are normalized.
Quat &Quat::SetFromToRotation(const Vector3 &from, const Vector3 &to)
{
    const float dot = Math::Clamp(from.Dot(to), -1.0f, 1.0f);
    if (dot >= 1.0f)
    {
        return SetIndentity();
    }

    const float rad = Math::Acos(dot);

    return FromAxisRad(from.Cross(to), rad);
}

Vector3 Quat::Rotate(const Vector3 &vec) const
{
    Quat conj = Conjugate();
    Quat temp = *this * Quat(vec.x, vec.y, vec.z, 0.0f) * conj;
	return Vector3(temp.x, temp.y, temp.z);
}

Matrix3 Quat::ToMatrix3() const
{
    const float xx = x * x;
    const float xy = x * y;
    const float xz = x * z;
    const float xw = x * w;
    const float yy = y * y;
    const float yz = y * z;
    const float yw = y * w;
    const float zz = z * z;
    const float zw = z * w;

    return {
        1.0f - 2.0f * (yy + zz),
        2.0f * (xy - zw),
        2.0f * (xz + yw),
        2.0f * (xy + zw),
        1.0f - 2.0f * (xx + zz),
        2.0f * (yz - xw),
        2.0f * (xz - yw),
        2.0f * (yz + xw),
        1.0f - 2.0f * (xx + yy)};
}

Matrix4 Quat::ToMatrix4() const
{
    const float xx = x * x;
    const float xy = x * y;
    const float xz = x * z;
    const float xw = x * w;
    const float yy = y * y;
    const float yz = y * z;
    const float yw = y * w;
    const float zz = z * z;
    const float zw = z * w;

    return {
        1.0f - 2.0f * (yy + zz),
        2.0f * (xy - zw),
        2.0f * (xz + yw),
        0.0f,
        2.0f * (xy + zw),
        1.0f - 2.0f * (xx + zz),
        2.0f * (yz - xw),
        0.0f,
        2.0f * (xz - yw),
        2.0f * (yz + xw),
        1.0f - 2.0f * (xx + yy),
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f};
}