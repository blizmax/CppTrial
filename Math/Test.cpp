#include "Math/Test.h"
#include "Core/Log.h"

static Log logger = Log(CT_TEXT("Math"));

void Math::TestVector2()
{
    Vector2 vec(-2.0f, 0.0f);
    logger.Info(CT_TEXT("x={0}, y={1}"), vec[0], vec.y);
}

void Math::Test()
{
    TestVector2();
}