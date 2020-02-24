#include "Reflection/Test.h"
#include "IO/Test.h"
#include "Math/Test.h"
#include "Json/Test.h"
#include "IO/FileWatcher.h"

int main()
{
    //Json::Test();
    //CT_EXCEPTION(Runtime, "Test Exception");

    using IO::FileWatcher;
    using IO::FileSystem;
    FileWatcher watcher(CT_TEXT("E:/Watcher"), [](const String &path, FileWatcher::FileStatus status){
        switch (status)
        {
        case FileWatcher::FileStatus::Changed:
            CT_LOG(Info, CT_TEXT("Changed. Is File:{0}"), FileSystem::IsFile(path));
            break;
        case FileWatcher::FileStatus::Created:
            CT_LOG(Info, CT_TEXT("Created. Is File:{0}"), FileSystem::IsFile(path));
            break;
        case FileWatcher::FileStatus::Deleted:
            CT_LOG(Info, CT_TEXT("Deleted. Is File:{0}"), FileSystem::IsFile(path));
            break;
        }
    });
    watcher.Start();
  
    system("pause");
    return 0;
}