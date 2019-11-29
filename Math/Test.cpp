#include "Math/Test.h"
#include "Core/Log.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"

static Log logger = Log(CT_TEXT("Math"));

void Math::TestVector2()
{
    Vector2 vec = Vector2(-2.0f, 0.0f) - Vector2(-5.0f, 2.0f);
    logger.Info(CT_TEXT("{0}"), vec);
}

void Math::TestVector3()
{
    
}

void Math::Test()
{
    TestVector2();
}