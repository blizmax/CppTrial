#include "IO/Test.h"
#include "Core/Logger.h"

void IO::TestFileHandle()
{
    FileHandle fileHandle(CT_TEXT("E:/Project/C++"));
    //path.Append(CT_TEXT("pic1.jpg"));
    CT_LOG(Info, CT_TEXT("{0} is directory? {1}"), fileHandle, fileHandle.IsDirectory());

    FileHandle fileHandle2(CT_TEXT("E:/TestPath/Dir1/"));
    bool ret = fileHandle2.CreateDirectory();
    CT_LOG(Info, CT_TEXT("{0} create dirs result {1}"), fileHandle2.GetAbsolutePath(), ret);

    CT_LOG(Info, CT_TEXT("{0} exists? {1}"), fileHandle2.GetParent().GetFullPath(), fileHandle2.GetParent().Exists());

    //FileHandle fileHandle3 = FileHandle(CT_TEXT("E:/vpn/vpn"));
    //auto children = fileHandle3.List(true);

    FileHandle fileHandle3 = FileHandle(CT_TEXT("E:/code"));
    auto children = fileHandle3.List([](const String &path) {
        return path.EndsWith(CT_TEXT(".zip"));
    });

    for (auto &child : children)
    {
        CT_LOG(Info, child.ToString());
    }

    FileHandle fileHandle4 = FileHandle(CT_TEXT("E:/dog.tree"));
    CT_LOG(Info, CT_TEXT("File size of {0} is : {1}"), fileHandle4, fileHandle4.GetSize());
    // logger.Info(path.GetExtension());
    // logger.Info(path.GetFileNameWithoutExtension());
    // logger.Info(path.GetFileName());
    // logger.Info(path.GetFullPath());
}


void IO::TestFileSystem()
{
    String p1 = CT_TEXT("D:Parent");
    String p2 = CT_TEXT("E:/code");
    CT_LOG(Info, CT_TEXT("{0} exists? {1}"), p1, FileSystem::Exists(p1));
    CT_LOG(Info, CT_TEXT("{0} exists? {1}"), p2, FileSystem::Exists(p2));

    CT_LOG(Info, CT_TEXT("{0} is dir?"), FileSystem::IsDirectory(CT_TEXT("E:/emp")));
}

void IO::TestFileStream()
{
    FileInputStream istream = FileHandle(CT_TEXT("E:/dog.tree")).Read();
    CT_LOG(Info, CT_TEXT("ifstream is open? {0}"), istream.IsOpen());
    String str = istream.ReadString();
    CT_LOG(Info, str);
    FileOutputStream ostream = FileHandle(CT_TEXT("E:/dog_copy.tree")).Write();
    CT_LOG(Info, CT_TEXT("ofstream is open? {0}"), ostream.IsOpen());
    ostream.WriteString(str);

    // FileInputStream istream = FileInputStream(CT_TEXT("E:/dog.tree"));
    // logger.Info(CT_TEXT("ifstream is open? {0}"), istream.IsOpen());
    // auto arr = istream.ReadBytes();

    // FileOutputStream ostream = FileOutputStream(CT_TEXT("E:/dog_copy.tree"));
    // logger.Info(CT_TEXT("ofstream is open? {0}"), ostream.IsOpen());
    // ostream.WriteBytes(arr);
}

void IO::Test()
{
    //TestFileHandle();
    //TestFileSystem();
    TestFileStream();
}