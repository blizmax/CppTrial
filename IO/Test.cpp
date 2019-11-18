#include "IO/Test.h"
#include "Core/Log.h"

static Log logger = Log(CT_TEXT("IO"));

void IO::TestFileHandle()
{
    FileHandle fileHandle(CT_TEXT("E:/Project/C++"));
    //path.Append(CT_TEXT("pic1.jpg"));
    logger.Info(CT_TEXT("{0} is directory? {1}"), fileHandle, fileHandle.IsDirectory());

    FileHandle fileHandle2(CT_TEXT("E:/TestPath/Dir1/"));
    bool ret = fileHandle2.CreateDirectory();
    logger.Info(CT_TEXT("{0} create dirs result {1}"), fileHandle2.GetAbsolutePath(), ret);

    logger.Info(CT_TEXT("{0} exists? {1}"), fileHandle2.GetParent().GetFullPath(), fileHandle2.GetParent().Exists());

    //FileHandle fileHandle3 = FileHandle(CT_TEXT("E:/vpn/vpn"));
    //auto children = fileHandle3.List(true);

    FileHandle fileHandle3 = FileHandle(CT_TEXT("E:/code"));
    auto children = fileHandle3.List([](const String& path){
        return path.EndsWith(CT_TEXT(".zip"));
    });

    for(auto& child : children)
    {
        logger.Info(child.ToString());
    }
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
    TestFileHandle();
    //TestFileSystem();
}