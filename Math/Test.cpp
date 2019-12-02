#include "Math/Test.h"
#include "Core/Log.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Rect.h"
#include "Math/Matrix3.h"
#include "Math/Matrix4.h"

static Log logger = Log(CT_TEXT("Math"));

void Math::TestVector2()
{
    Vector2 vec = Vector2(-2.0f, 0.0f) - Vector2(-5.0f, 2.0f);
    logger.Info(CT_TEXT("{0}"), vec);
}

void Math::TestVector3()
{
    
}

void Math::TestVector4()
{

}

void Math::TestRect()
{
    
}

void Math::TestMatrix3()
{
    Matrix3 mat;
    mat.SetColumn(0, Vector3(1.0f, 2.0f, 3.0f));
    logger.Info(CT_TEXT("Matrix3:{0}, det:{1}"), mat, mat.Determinant());
}

void Math::TestMatrix4()
{
    
}

void Math::Test()
{
    //TestVector2();
    TestMatrix3();
}