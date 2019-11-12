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


void IO::TestFileSystem()
{
    Path p1 = Path(CT_TEXT("D:Parent"));
    Path p2 = Path(CT_TEXT("E:/code"));
    logger.Info(CT_TEXT("{0} exists? {1}"), p1, FileSystem::Exists(p1));
    logger.Info(CT_TEXT("{0} exists? {1}"), p2, FileSystem::Exists(p2));
}

void IO::Test()
{
    //TestPath();
    TestFileSystem();
}