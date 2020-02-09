#include "Math/Test.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quat.h"
#include "Math/Rect.h"
#include "Math/Circle.h"
#include "Math/Sphere.h"
#include "Math/Ray.h"
#include "Math/Matrix3.h"
#include "Math/Matrix4.h"
#include "Core/Logger.h"

static void TestVector2()
{
    Vector2 vec = Vector2(-2.0f, 0.0f) - Vector2(-5.0f, 2.0f);
    CT_LOG(Info, CT_TEXT("{0}"), vec);
}

static void TestVector3()
{
    
}

static void TestVector4()
{

}

static void TestRect()
{
    
}

static void TestCircle()
{

}

static void TestSphere()
{
    Sphere sphere;
    CT_LOG(Info, CT_TEXT("Sphere:{0}"), sphere);
}

static void TestRay()
{
    
}

static void TestQuat()
{
    
}

static void TestMatrix3()
{
    Matrix3 mat;
    mat.SetColumn(0, Vector3(1.0f, 2.0f, 3.0f));
    CT_LOG(Info, CT_TEXT("Matrix3:{0}, det:{1}"), mat, mat.Determinant());
}

static void TestMatrix4()
{
    
}

void Test::TestMath()
{
    //TestVector2();

    TestSphere();

    //TestMatrix3();
}