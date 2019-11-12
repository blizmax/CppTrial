#include "IO/Test.h"
#include "Core/Log.h"

static Log logger = Log(CT_TEXT("IO"));

void IO::TestPath()
{
    Path path(CT_TEXT("D:Parent"));
    path.Append(CT_TEXT("pic1.jpg"));
    logger.Info(path.GetExtension());
    logger.Info(path.GetFileNameWithoutExtension());
    logger.Info(path.GetFileName());
    logger.Info(path.GetFullPath());
}

void IO::Test()
{
    TestPath();
}