#include "Reflection/Test.h"
#include "IO/Test.h"
#include "Math/Test.h"
#include "Json/Test.h"

int main()
{
    Json::Test();
    CT_EXCEPTION(Runtime, "Test Exception");
  
    system("pause");
    return 0;
}