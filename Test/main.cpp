#include "Reflection/Test.h"
#include "IO/Test.h"
#include "Math/Test.h"
#include "Json/Test.h"
#include "IO/FileWatcher.h"
#include "Test/TestCoreLib.h"

#include "IO/FileStream.h"
#include <fstream>

void TestEncode()
{
    // std::ios::openmode mode = std::ios::binary | std::ios::in;
    // std::ifstream fstream;
    // fstream.open("D:/test1.json", mode);
    // fstream.seekg(0, std::ios::end);
    // auto size = fstream.tellg();
    // fstream.seekg(0, std::ios::beg);

    // CT_LOG(Debug, CT_TEXT("open:{0},size:{1}"), fstream.is_open(), static_cast<int32>(size));

    IO::FileInputStream fstream(CT_TEXT("D:/test1.json"));
    auto bytes = fstream.ReadBytes();
    auto str = fstream.ReadString();
    fstream.Close();

    IO::FileOutputStream ostream(CT_TEXT("D:/test1.json"));
    ostream.WriteString(str);

    CT_LOG(Debug, CT_TEXT("open:{0},size:{1}"), fstream.IsOpen(), bytes.Count());
}

int main()
{
    //Json::Test();
    //CT_EXCEPTION(Runtime, "Test Exception");

    // using IO::FileWatcher;
    // using IO::FileSystem;
    // FileWatcher watcher(CT_TEXT("E:/Watcher"), [](const String &path, FileWatcher::FileStatus status){
    //     switch (status)
    //     {
    //     case FileWatcher::FileStatus::Changed:
    //         CT_LOG(Info, CT_TEXT("Changed. Is File:{0}"), FileSystem::IsFile(path));
    //         break;
    //     case FileWatcher::FileStatus::Created:
    //         CT_LOG(Info, CT_TEXT("Created. Is File:{0}"), FileSystem::IsFile(path));
    //         break;
    //     case FileWatcher::FileStatus::Deleted:
    //         CT_LOG(Info, CT_TEXT("Deleted. Is File:{0}"), FileSystem::IsFile(path));
    //         break;
    //     }
    // });
    // watcher.Start();

    //Test::TestDelegate();
  
    //TestEncode();

    //Test::TestArray();
    Test::TestHashMap();
    //Test::TestTuple();

    system("pause");
    return 0;
}