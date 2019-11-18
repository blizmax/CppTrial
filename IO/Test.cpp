#include "IO/Test.h"
#include "Core/Log.h"

static Log logger = Log(CT_TEXT("IO"));

void IO::TestPath()
{
    Path path(CT_TEXT("E:/Project/C++"));
    //path.Append(CT_TEXT("pic1.jpg"));
    logger.Info(CT_TEXT("{0} is directory? {1}"), path, path.IsDirectory());

    Path path2(CT_TEXT("E:/TestPath/Dir1/"));
    bool ret = path2.CreateDirectory();
    logger.Info(CT_TEXT("{0} create dirs result {1}"), path2.GetAbsolute(), ret);

    logger.Info(CT_TEXT("{0} exists? {1}"), path2.GetParentPath().GetAbsolute(), path2.GetParentPath().Exists());

    // logger.Info(path.GetExtension());
    // logger.Info(path.GetFileNameWithoutExtension());
    // logger.Info(path.GetFileName());
    // logger.Info(path.GetFullPath());
}


void IO::TestFileSystem()
{
    String p1 = CT_TEXT("D:Parent");
    String p2 = CT_TEXT("E:/code");
    logger.Info(CT_TEXT("{0} exists? {1}"), p1, FileSystem::Exists(p1));
    logger.Info(CT_TEXT("{0} exists? {1}"), p2, FileSystem::Exists(p2));

    logger.Info(CT_TEXT("{0} is dir?"), FileSystem::IsDirectory(CT_TEXT("E:/emp")));
}

void IO::Test()
{
    TestPath();
    //TestFileSystem();
}